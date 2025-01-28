#include "monitor_utils.h"
#include <stdint.h>
#include <math.h>

uint64_t get_sampling_interval(MonitorConfig *config, int iteration) {
    if (config->customDataCollect) {
        if (iteration == config->numSamples - 1) {
            if (config->sampleInterval < 0.0625) {
                return 1000000.0;
            }

            config->numSamples = config->numSamples * 2;
            config->sampleInterval = config->sampleInterval / 2;
        }

        return (uint64_t)(config->sampleInterval * 1000000.0);
    } else {
        // Straight multiply the sampleInterval by 1e6 for microseconds
        return (uint64_t)(config->sampleInterval * 1000000.0);
    }
}
