#include "monitor_utils.h"
#include <stdint.h>

static int round = 1;

uint64_t get_sampling_interval_by_time(MonitorConfig *config, double sec_elapsed) {
    if (config->customDataCollect) {
        // Add custom rules here

        if (sec_elapsed > 5 * round) {
            config->sampleInterval /= 2;
            round += 1;
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
