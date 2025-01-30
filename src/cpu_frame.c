#include "cpu_frame.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CpuUsageFrame parse_cpu_frame(const char *line,
                              int8_t cpu_id,
                              uint64_t timestep,
                              uint64_t sampling_interval,
                              CpuFrame *prev_frame,
                              int enable_all_fields) {
    CpuFrame cur_frame;
    memset(&cur_frame, 0, sizeof(cur_frame));

    unsigned long long u=0, n=0, s=0, i=0, io=0, ir=0, so=0, st=0, g=0, gn=0;
    {
        char dummy[16];
        int scanned = sscanf(line,
            "%15s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
            dummy, &u, &n, &s, &i, &io, &ir, &so, &st, &g, &gn
        );
        if (scanned < 4) {
            // We failed to read the minimum user/system/idle
        }
    }

    cur_frame.user       = u;
    cur_frame.nice       = n;
    cur_frame.system     = s;
    cur_frame.idle       = i;
    cur_frame.iowait     = io;
    cur_frame.irq        = ir;
    cur_frame.softirq    = so;
    cur_frame.steal      = st;
    cur_frame.guest      = g;
    cur_frame.guest_nice = gn;

    CpuUsageFrame usage_frame;
    memset(&usage_frame, 0, sizeof(usage_frame));

    usage_frame.cpu_id            = cpu_id;
    usage_frame.timestep          = timestep;
    usage_frame.sampling_interval = sampling_interval;

    if (timestep == 0) {
        *prev_frame = cur_frame;
        return usage_frame;
    }

    unsigned long long user_diff   = cur_frame.user       - prev_frame->user;
    unsigned long long nice_diff   = cur_frame.nice       - prev_frame->nice;
    unsigned long long sys_diff    = cur_frame.system     - prev_frame->system;
    unsigned long long idle_diff   = cur_frame.idle       - prev_frame->idle;
    unsigned long long iowait_diff = cur_frame.iowait     - prev_frame->iowait;
    unsigned long long irq_diff    = cur_frame.irq        - prev_frame->irq;
    unsigned long long soft_diff   = cur_frame.softirq    - prev_frame->softirq;
    unsigned long long steal_diff  = cur_frame.steal      - prev_frame->steal;
    unsigned long long guest_diff  = cur_frame.guest      - prev_frame->guest;
    unsigned long long gnice_diff  = cur_frame.guest_nice - prev_frame->guest_nice;

    unsigned long long total_diff =
        user_diff + sys_diff + idle_diff +
        nice_diff + iowait_diff + irq_diff +
        soft_diff + steal_diff + guest_diff + gnice_diff;

    if (total_diff > 0) {
        usage_frame.user   = (uint8_t)((user_diff   * 100) / total_diff);
        usage_frame.system = (uint8_t)((sys_diff    * 100) / total_diff);
        usage_frame.idle   = (uint8_t)((idle_diff   * 100) / total_diff);
        if (enable_all_fields) {
            usage_frame.nice   = (uint8_t)((nice_diff   * 100) / total_diff);
            usage_frame.iowait = (uint8_t)((iowait_diff * 100) / total_diff);
            usage_frame.irq    = (uint8_t)((irq_diff    * 100) / total_diff);
            usage_frame.softirq= (uint8_t)((soft_diff   * 100) / total_diff);
            usage_frame.steal  = (uint8_t)((steal_diff  * 100) / total_diff);
            usage_frame.guest  = (uint8_t)((guest_diff  * 100) / total_diff);
            usage_frame.guest_nice = (uint8_t)((gnice_diff * 100) / total_diff);
        }
    }

    *prev_frame = cur_frame;
    return usage_frame;
}