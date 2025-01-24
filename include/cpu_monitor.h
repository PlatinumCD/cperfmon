#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <stdint.h>
#include "monitor_config.h"

/**
 * Initializes the CPU monitor: sets up reading /proc/stat,
 * prepares any output mechanism (plugin or CSV),
 * allocates necessary resources, etc.
 */
void init_cpu_monitor(const MonitorConfig *config);

/**
 * Reads /proc/stat, parses CPU lines, computes usage frames,
 * and sends them to the chosen output. 
 */
void sample_cpu_data(uint64_t timestep, uint64_t sampling_interval);

/**
 * Finalizes the CPU monitor: flushes/publishes any pending data,
 * closes files, frees allocated resources, etc.
 */
void finalize_cpu_monitor(void);

#endif // CPU_MONITOR_H