C-PerfMon Library
=========================

Overview
--------
This document outlines the approach for a CWaggle Performance Monitor. The library employs a configuration-based methodology to facilitate field parsing, data publishing to Waggle, and custom data collecting intervals.

Build and Execution
-------------------
To build and run the application, execute the following commands:

```
make
./perfmon
```

Configuration via Environment Variables
---------------------------------------
The behavior of the C-PerfMon library can be customized using the following optional environment variables:

- `PERFMON_ENABLE_ALL_FIELDS=1`: Enables all available fields for monitoring.
- `PERFMON_PUBLISH_DATA=1`: Activates data publishing to Waggle.
- `PERFMON_NUMBER_SAMPLES=200`: Sets the number of samples to collect.
- `PERFMON_SAMPLE_INTERVAL=0.1`: Specifies the interval between samples in seconds.
- `PERFMON_CUSTOM_DATA_COLLECT=1`: Enables custom data collection method.
- `PERFMON_ONLY_COLLECT_AGGREGATE=1`: Only collects data from aggregate CPU values.

Important Note
--------------
`PERFMON_CUSTOM_DATA_COLLECT` is designed for custom data collection, which operates on a user programmed interval. In contrast, `PERFMON_SAMPLE_INTERVAL` allows for a custom fix sampling interval. Using both simultaneously may result in conflicts in data collection timing and behavior, as they cater to different operational modes.

These environment variables provide control over extended fields, plugin publishing, and advanced data-collecting logic.
