#include "cpu_output.h"
#include "waggle/plugin.h"
#include "waggle/config.h"
#include "waggle/timeutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    Plugin *p;
} PluginOutputData;

static void plugin_init(CpuOutput *self) {
    PluginOutputData *d = (PluginOutputData *)self->data;

    const char *username = getenv("WAGGLE_PLUGIN_USERNAME");
    if (!username) username = "plugin";

    const char *password = getenv("WAGGLE_PLUGIN_PASSWORD");
    if (!password) password = "plugin";

    const char *host = getenv("WAGGLE_PLUGIN_HOST");
    if (!host) host = "rabbitmq";

    const char *port_str = getenv("WAGGLE_PLUGIN_PORT");
    int port = port_str ? atoi(port_str) : 5672;

    const char *app_id = getenv("WAGGLE_APP_ID");
    if (!app_id) app_id = "";

    PluginConfig *cfg = plugin_config_new(username, password, host, port, app_id);
    if (!cfg) {
        fprintf(stderr, "Failed to create PluginConfig.\n");
        exit(EXIT_FAILURE);
    }

    d->p = plugin_new(cfg);
    if (!d->p) {
        fprintf(stderr, "Failed to create Plugin.\n");
        exit(EXIT_FAILURE);
    }
}

static void plugin_write_frame(CpuOutput *self, const CpuUsageFrame *frame) {
    PluginOutputData *d = (PluginOutputData *)self->data;
    if (!d->p) {
        return;
    }

    int enable_all = self->enable_all;
    uint64_t timestamp = waggle_get_timestamp_ns();

    char metadata[64];
//    snprintf(metadata, sizeof(metadata), "{\"si\": \"%" PRIu64 "\"}", frame->sampling_interval);
    snprintf(metadata, sizeof(metadata), "{}");
     

    if (enable_all) {
        const char *fields[] = {
            "user", "nice", "system", "idle", "iowait",
            "irq", "softirq", "steal", "guest", "guest_nice"
        };
        uint8_t values[] = {
            frame->user, frame->nice, frame->system, frame->idle,
            frame->iowait, frame->irq, frame->softirq, frame->steal,
            frame->guest, frame->guest_nice
        };

        for (int i = 0; i < 10; i++) {
            char topic[64];
            if (frame->cpu_id < 0) {
                snprintf(topic, sizeof(topic), "perf.cpu.%s", fields[i]);
            } else {
                snprintf(topic, sizeof(topic), "perf.cpu%d.%s", frame->cpu_id, fields[i]);
            }
            plugin_publish(d->p, "all", topic, values[i], timestamp, metadata);
        }
    } else {
        const char *fields[] = {"user", "system", "idle"};
        uint8_t values[] = {frame->user, frame->system, frame->idle};

        for (int i = 0; i < 3; i++) {
            char topic[64];
            if (frame->cpu_id < 0) {
                snprintf(topic, sizeof(topic), "perf.cpu.%s", fields[i]);
            } else {
                snprintf(topic, sizeof(topic), "perf.cpu%d.%s", frame->cpu_id, fields[i]);
            }
            plugin_publish(d->p, "all", topic, values[i], timestamp, metadata);
        }
    }
}

static void plugin_flush(CpuOutput *self) {
    (void)self;
}

static void plugin_close(CpuOutput *self) {
    PluginOutputData *d = (PluginOutputData *)self->data;
    if (d->p) {
        plugin_free(d->p);
        d->p = NULL;
    }
    free(d);
    self->data = NULL;
}

CpuOutput *create_plugin_output(int enable_all) {
    CpuOutput *out = (CpuOutput *)malloc(sizeof(CpuOutput));
    PluginOutputData *data = (PluginOutputData *)malloc(sizeof(PluginOutputData));
    data->p = NULL;

    out->init        = plugin_init;
    out->write_frame = plugin_write_frame;
    out->flush       = plugin_flush;
    out->close       = plugin_close;
    out->data        = data;
    out->enable_all  = enable_all;
    return out;
}
