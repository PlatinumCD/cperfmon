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

    if (getenv("PERFMON_SAMPLE_INTERVAL")) {
        char *endptr;
        float sampleInterval = strtof(getenv("PERFMON_SAMPLE_INTERVAL"), &endptr);
        if (*endptr == '\0') {
            config.sampleInterval = sampleInterval;
        }
    } else {
        config.sampleInterval = 1.0f;
    }

    if (getenv("PERFMON_ONLY_COLLECT_AGGREGATE")) {
        if (strcmp(getenv("PERFMON_ONLY_COLLECT_AGGREGATE"), "1") == 0) {
            config.onlyCollectAggregate = 1;
        }
    }

    if (getenv("PERFMON_CUSTOM_DATA_COLLECT")) {
        if (strcmp(getenv("PERFMON_CUSTOM_DATA_COLLECT"), "1") == 0) {
            config.customDataCollect = 1;
        }
    }

    if (getenv("PERFMON_NUMBER_SAMPLES") && !getenv("PERFMON_DURATION")) {
        char *endptr;
        long numSamples = strtol(getenv("PERFMON_NUMBER_SAMPLES"), &endptr, 10);
        if (*endptr == '\0') {
            config.numSamples = numSamples;
            config.runDuration = 0.0f;
        }
    } else if (getenv("PERFMON_DURATION") && !getenv("PERFMON_NUMBER_SAMPLES")) {
        char *endptr;
        float runDuration = strtof(getenv("PERFMON_DURATION"), &endptr);
        if (*endptr == '\0') {
            config.runDuration = runDuration;
        }
        config.numSamples = 0;
    } else {
        config.numSamples = 100;
        config.runDuration = 0.0f;
    }

    return config;
}
