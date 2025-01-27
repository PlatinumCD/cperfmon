#include "monitor_utils.h"
#include <stdint.h>
#include <math.h>

uint64_t get_sampling_interval(const MonitorConfig *config, int iteration) {
    if (config->customDataCollect) {

        if (iteration == 8) {
            return 1000000;
        }

        if (iteration == 230399) {
            return 1000000;
        }

        double val;
        if (iteration == config->numSamples - 1) {
            config->sampleInterval /= 2;
            config->numSamples *= 2;
            val = 1.0;
        }

        return (uint64_t)(val * 1000000.0);
    } else {
        // Straight multiply the sampleInterval by 1e6 for microseconds
        return (uint64_t)(config->sampleInterval * 1000000.0);
    }
}
