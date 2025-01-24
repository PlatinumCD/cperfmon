Rewritten PerfMon Library
=========================

This code demonstrates a reorganized approach for the CPU performance monitor,
inspired by large-scale software design principles. It uses a config-based
approach instead of preprocessor directives to manage extended field parsing,
publishing to Waggle, or custom data harvesting intervals.

Build and Run:
--------------
make
./perfmon

Optional Env Variables:
-----------------------
PERFMON_ENABLE_ALL_FIELDS=1
PERFMON_PUBLISH_DATA=1
PERFMON_NUMBER_SAMPLES=200
PERFMON_SAMPLE_INTERVAL=0.1
PERFMON_SIN_DATA_COLLECT=1

Note: The environment variables `PERFMON_SIN_DATA_COLLECT` and `PERFMON_SAMPLE_INTERVAL` cannot both be used simultaneously. `PERFMON_SIN_DATA_COLLECT` is intended for enabling sine wave data collection, which operates on a fixed interval, while `PERFMON_SAMPLE_INTERVAL` specifies a custom sampling interval for data collection. Using both may lead to conflicts in data collection timing and behavior, as they are designed for different operational modes.

These environment variables control the extended fields, plugin publishing, 
and advanced data-harvest logic respectively.
