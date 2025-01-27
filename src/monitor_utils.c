#include "monitor_utils.h"
#include <stdint.h>
#include <math.h>

uint64_t get_sampling_interval(MonitorConfig *config, int iteration) {
    if (config->customDataCollect) {

        double val = 1.0;

        return (uint64_t)(val * 1000000.0);
    } else {
        // Straight multiply the sampleInterval by 1e6 for microseconds
        return (uint64_t)(config->sampleInterval * 1000000.0);
    }
}
