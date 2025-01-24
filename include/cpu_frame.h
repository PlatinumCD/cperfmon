#ifndef CPU_FRAME_H
#define CPU_FRAME_H

#include <stdint.h>

/**
 * Raw CPU counters (straight from /proc/stat).
 * We store all fields unconditionally to keep
 * the code simpler and more extensible.
 */
typedef struct {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    unsigned long long guest;
    unsigned long long guest_nice;
} CpuFrame;

/**
 * Percent usage CPU structure.
 * This is what we ultimately want to record or log.
 * All fields are stored. If some fields are not needed,
 * they can be used as zero.
 */
typedef struct {
    int8_t   cpu_id;
    uint64_t timestep;
    uint64_t sampling_interval;

    // Usage in % (0..100)
    uint8_t user;
    uint8_t nice;
    uint8_t system;
    uint8_t idle;
    uint8_t iowait;
    uint8_t irq;
    uint8_t softirq;
    uint8_t steal;
    uint8_t guest;
    uint8_t guest_nice;
} CpuUsageFrame;

/**
 * parse_cpu_frame:
 *    Parses one line of /proc/stat and computes usage (%) by
 *    comparing with the previous raw stats. Returns a CpuUsageFrame.
 *
 *    enable_all_fields: if true, compute usage for all fields.
 *                       if false, only compute user/system/idle.
 */
CpuUsageFrame parse_cpu_frame(const char *line,
                              int8_t cpu_id,
                              uint64_t timestep,
                              uint64_t sampling_interval,
                              CpuFrame *prev_frame,
                              int enable_all_fields);

#endif // CPU_FRAME_H