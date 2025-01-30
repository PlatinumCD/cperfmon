#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>

#include "monitor_config.h"
#include "monitor_utils.h"
#include "cpu_monitor.h"

int main(void) {
    MonitorConfig config = load_monitor_config();
    init_cpu_monitor(&config);

    if (config.runDuration > 0.0f) {
        struct timespec start, now;
        clock_gettime(CLOCK_MONOTONIC, &start);
        uint64_t timestep = 0;

        while (1) {
            clock_gettime(CLOCK_MONOTONIC, &now);
            double elapsed = (now.tv_sec - start.tv_sec)
                           + (now.tv_nsec - start.tv_nsec) / 1e9;

            if (elapsed >= config.runDuration) {
                break;
            }

            uint64_t interval = get_sampling_interval_by_time(&config, elapsed);
            sample_cpu_data(timestep, interval);
            usleep(interval);

            timestep++;
        }
    } else {
        uint64_t timestep = 0;
        for (int iteration = 0; iteration < config.numSamples; iteration++) {
            uint64_t interval = get_sampling_interval_by_iteration(&config, iteration);
            sample_cpu_data(timestep, interval);
            usleep(interval);

            timestep++;
        }
    }

    finalize_cpu_monitor();
    return 0;
}
