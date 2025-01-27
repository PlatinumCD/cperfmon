#define _GNU_SOURCE
#include "cpu_monitor.h"
#include "cpu_frame.h"
#include "cpu_output.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

static CpuFrame  *prev_frames    = NULL;
static int        proc_stat_fd   = -1;
static int        num_cpus       = 0;
static CpuOutput *output         = NULL;
static int        enable_all     = 0;
static int        only_aggregate = 0;

#define BUFSZ 8192

static char cpu_data_buf[BUFSZ];

// --------------------------------------------------------------------------
// Forward declarations for private functions
// --------------------------------------------------------------------------
static void open_proc_stat(void);
static void close_proc_stat(void);
static void determine_num_cpus(void);

// --------------------------------------------------------------------------
// Public interface
// --------------------------------------------------------------------------
void init_cpu_monitor(const MonitorConfig *config) {
    enable_all = config->enableAllFields;
    only_aggregate = config->onlyCollectAggregate;

    // Detemine whether we only collect aggregate CPU usage or collect data across all cores
    if (only_aggregate) {
        num_cpus = 0;
    } else {
        determine_num_cpus();
    }

    // Open /proc/stat
    open_proc_stat();

    // Allocate space for previous frames (including an extra for aggregate CPU)
    if (only_aggregate) {
        prev_frames = (CpuFrame *)calloc(1, sizeof(CpuFrame));
    } else {
        prev_frames = (CpuFrame *)calloc(num_cpus + 1, sizeof(CpuFrame));
        if (!prev_frames) {
            perror("calloc for prev_frames");
            exit(EXIT_FAILURE);
        }
    }

    // We create an output object (CSV or Plugin) based on publishData
    if (config->publishData) {
        extern CpuOutput *create_plugin_output(int);
        output = create_plugin_output(enable_all);
    } else {
        extern CpuOutput *create_csv_output(int);
        output = create_csv_output(enable_all);
    }

    output->init(output);
}

// parse lines from /proc/stat and pass usage frames to output
void sample_cpu_data(uint64_t timestep, uint64_t sampling_interval) {
    if (proc_stat_fd < 0) {
        return;
    }
    lseek(proc_stat_fd, 0, SEEK_SET);
    ssize_t n = read(proc_stat_fd, cpu_data_buf, BUFSZ - 1);
    if (n <= 0) {
        fprintf(stderr, "Error reading /proc/stat: %s\n", strerror(errno));
        return;
    }
    cpu_data_buf[n] = '\0';

    // We skip the first "cpu " line if we want to read per-core lines,
    // or we treat the first line as the 'aggregate' CPU with index = -1.
    // We'll parse up to num_cpus+1 lines: the first is "cpu", then "cpu0","cpu1",...
    int cpu_line_index = 0;
    char *saveptr = NULL;
    char *line = strtok_r(cpu_data_buf, "\n", &saveptr);

    while (line) {
        // If it starts with "cpu", let's parse it.
        if (strncmp(line, "cpu", 3) == 0) {
            if (cpu_line_index > num_cpus) {
                // We don't parse more than one aggregate + all cores
                break;
            }
            CpuUsageFrame frame = parse_cpu_frame(line,
                                                  (int8_t)(cpu_line_index - 1),
                                                  timestep,
                                                  sampling_interval,
                                                  &prev_frames[cpu_line_index],
                                                  enable_all);
            output->write_frame(output, &frame);
            cpu_line_index++;
        }
        line = strtok_r(NULL, "\n", &saveptr);
    }
}

void finalize_cpu_monitor(void) {
    if (output) {
        output->flush(output);
        output->close(output);
        output = NULL;
    }
    close_proc_stat();
    if (prev_frames) {
        free(prev_frames);
        prev_frames = NULL;
    }
}

// --------------------------------------------------------------------------
// Private helpers
// --------------------------------------------------------------------------
static void determine_num_cpus(void) {
    // Query the CPU affinity to get the number of CPUs
    // Alternatively, we could parse /proc/cpuinfo, but this is simpler.
    cpu_set_t cs;
    CPU_ZERO(&cs);
    if (sched_getaffinity(0, sizeof(cs), &cs) == 0) {
        num_cpus = CPU_COUNT(&cs);
    }
}

static void open_proc_stat(void) {
    proc_stat_fd = open("/proc/stat", O_RDONLY);
    if (proc_stat_fd < 0) {
        perror("open /proc/stat");
        exit(EXIT_FAILURE);
    }
}

static void close_proc_stat(void) {
    if (proc_stat_fd >= 0) {
        close(proc_stat_fd);
        proc_stat_fd = -1;
    }
}
