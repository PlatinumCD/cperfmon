#include "monitor_utils.h"
#include <stdint.h>
#include <math.h>

static int round_count = 1;

uint64_t get_sampling_interval_by_time(MonitorConfig *config, double sec_elapsed) {
    if (config->customDataCollect) {
        // Add custom rules here

#        if (sec_elapsed > 3600 * round_count) {
        if (sec_elapsed > 20 * round_count) {
            round_count += 1;
            config->sampleInterval = exp((double)(round_count) / -5.0) * 4.0;
    	    return (uint64_t)(1000000.0);
        }
    }

    return (uint64_t)(config->sampleInterval * 1000000.0);
}

uint64_t get_sampling_interval_by_iteration(MonitorConfig *config, int iteration) {
    if (config->customDataCollect) {
        // Add custom rules here
        config->sampleInterval -= iteration * 1e-9;
    }

    return (uint64_t)(config->sampleInterval * 1000000.0);
}
