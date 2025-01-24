#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>

#include "monitor_config.h"
#include "monitor_utils.h"
#include "cpu_monitor.h"

int main(void) {
    MonitorConfig config = load_monitor_config();
    init_cpu_monitor(&config);

    uint64_t timestep = 0;
    for (int i = 0; i < config.numSamples; i++) {
        uint64_t interval = get_sampling_interval(&config, i);
        sample_cpu_data(timestep, interval);
        usleep(interval);
        timestep++;
    }

    finalize_cpu_monitor();
    return 0;
}
