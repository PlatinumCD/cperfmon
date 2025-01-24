#include "monitor_config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

MonitorConfig load_monitor_config(void) {
    MonitorConfig config;
    memset(&config, 0, sizeof(config));

    if (getenv("PERFMON_ENABLE_ALL_FIELDS")) {
        if (strcmp(getenv("PERFMON_ENABLE_ALL_FIELDS"), "1") == 0) {
            config.enableAllFields = 1;
        }
    }

    if (getenv("PERFMON_PUBLISH_DATA")) {
        if (strcmp(getenv("PERFMON_PUBLISH_DATA"), "1") == 0) {
            config.publishData = 1;
        }
    }

    if (getenv("PERFMON_NUMBER_SAMPLES")) {
        char *endptr;
        long numSamples = strtol(getenv("PERFMON_NUMBER_SAMPLES"), &endptr, 10);
        if (*endptr == '\0') {
            config.numSamples = numSamples;
        }
    } else {
        config.numSamples = 100;
    }

    if (getenv("PERFMON_SIN_DATA_COLLECT") && getenv("PERFMON_SAMPLE_INTERVAL")) {
        // Handle error: both cannot be specified at the same time
        fprintf(stderr, "Error: PERFMON_SIN_DATA_COLLECT and PERFMON_SAMPLE_INTERVAL cannot be specified at the same time.\n");
        exit(EXIT_FAILURE);
    }

    if (getenv("PERFMON_SIN_DATA_COLLECT")) {
        if (strcmp(getenv("PERFMON_SIN_DATA_COLLECT"), "1") == 0) {
            config.sinDataCollect = 1;
        }
    } else if (getenv("PERFMON_SAMPLE_INTERVAL")) {
        char *endptr;
        float sampleInterval = strtof(getenv("PERFMON_SAMPLE_INTERVAL"), &endptr);
        if (*endptr == '\0') {
            config.sampleInterval = sampleInterval;
        }
    } else {
        config.sampleInterval = 1.0f;
    }


    return config;
}
