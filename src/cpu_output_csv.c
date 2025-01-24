#include "cpu_output.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
    FILE *csv_file;
} CsvOutputData;

static void csv_init(CpuOutput *self) {
    CsvOutputData *d = (CsvOutputData *)self->data;
    int enable_all = self->enable_all;
    d->csv_file = fopen("cpu_stats.csv", "w");
    if (!d->csv_file) {
        perror("fopen: cpu_stats.csv");
        exit(EXIT_FAILURE);
    }
    // Write CSV header (includes all fields)
    
    if (enable_all) {
        fprintf(d->csv_file, "cpu_id,timestep,sampling_interval,user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice\n");
    } else {
        fprintf(d->csv_file, "cpu_id,timestep,sampling_interval,user,system,idle\n");
    }
}

static void csv_write_frame(CpuOutput *self, const CpuUsageFrame *frame) {
    CsvOutputData *d = (CsvOutputData *)self->data;
    if (!d->csv_file) {
        return;
    }

    int enable_all = self->enable_all;
    if (enable_all) {
        fprintf(d->csv_file,
            "%d,%" PRIu64 ",%" PRIu64 ",%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
            frame->cpu_id,
            frame->timestep,
            frame->sampling_interval,
            frame->user,
            frame->nice,
            frame->system,
            frame->idle,
            frame->iowait,
            frame->irq,
            frame->softirq,
            frame->steal,
            frame->guest,
            frame->guest_nice
        );
    } else {
        fprintf(d->csv_file,
            "%d,%" PRIu64 ",%" PRIu64 ",%u,%u,%u\n",
            frame->cpu_id,
            frame->timestep,
            frame->sampling_interval,
            frame->user,
            frame->system,
            frame->idle
        );
    }
}

static void csv_flush(CpuOutput *self) {
    CsvOutputData *d = (CsvOutputData *)self->data;
    if (!d->csv_file) {
        return;
    }
    fflush(d->csv_file);
}

static void csv_close(CpuOutput *self) {
    CsvOutputData *d = (CsvOutputData *)self->data;
    if (d->csv_file) {
        fclose(d->csv_file);
        d->csv_file = NULL;
    }
    free(d);
    self->data = NULL;
}

CpuOutput *create_csv_output(int enable_all) {
    CpuOutput *out = (CpuOutput *)malloc(sizeof(CpuOutput));
    CsvOutputData *data = (CsvOutputData *)malloc(sizeof(CsvOutputData));

    out->init = csv_init;
    out->write_frame = csv_write_frame;
    out->flush = csv_flush;
    out->close = csv_close;
    out->data = data;
    out->enable_all = enable_all;
    return out;
}
