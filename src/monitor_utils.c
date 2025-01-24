#include "monitor_utils.h"
#include <stdint.h>
#include <math.h>

uint64_t get_sampling_interval(const MonitorConfig *config, int iteration) {
    if (config->sinDataCollect) {
        double x = (double)iteration;
        // Original formula:
        //   y = e^(cos(x/10) * 4) / 15 * 1e6
        double val = exp(cos(x / 10.0) * 4.0) / 15.0;
        return (uint64_t)(val * 1000000.0);
    } else {
        // Straight multiply the sampleInterval by 1e6 for microseconds
        return (uint64_t)(config->sampleInterval * 1000000.0);
    }
}
