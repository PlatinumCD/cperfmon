#ifndef CPU_OUTPUT_H
#define CPU_OUTPUT_H

#include "cpu_frame.h"

typedef struct CpuOutput CpuOutput;

/**
 * Generic interface for output (CSV, plugin, etc.).
 */
struct CpuOutput {
    void (*init)(CpuOutput *self);
    void (*write_frame)(CpuOutput *self, const CpuUsageFrame *frame);
    void (*flush)(CpuOutput *self);
    void (*close)(CpuOutput *self);
    void *data; // Implementation-specific
    int enable_all;
};

#endif // CPU_OUTPUT_H