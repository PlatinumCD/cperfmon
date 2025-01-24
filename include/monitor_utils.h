#ifndef MONITOR_UTILS_H
#define MONITOR_UTILS_H

#include <stdint.h>
#include "monitor_config.h"

/**
 * get_sampling_interval:
 *   If dataHarvest is enabled, compute a fancy time interval,
 *   otherwise use the sampleInterval in microseconds.
 *   iteration is used to change the sampling interval in the fancy mode.
 */
uint64_t get_sampling_interval(const MonitorConfig *config, int iteration);

#endif // MONITOR_UTILS_H
