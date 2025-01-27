#ifndef MONITOR_CONFIG_H
#define MONITOR_CONFIG_H

typedef struct {
    int enableAllFields;   // if non-zero, parse and compute extended fields
    int publishData;       // if non-zero, publish via Waggle plugin
    int customDataCollect; // if non-zero, use a fancy approach to sampling interval
    int onlyCollectAggregate; // if non-zero, only collect aggregate CPU values
    int numSamples;
    float sampleInterval; // base sampling interval for standard mode
} MonitorConfig;

/**
 * Loads configuration by checking environment variables.
 * The variables, if set to "1", will be enabled.
 * sampleInterval can be set to any float you desire as a base rate.
 */
MonitorConfig load_monitor_config(void);
void update_sampling_interval(MonitorConfig*, float);

#endif // MONITOR_CONFIG_H
