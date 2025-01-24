FROM platinumcd/plugin-cwaggle-base:1.0.0-base

WORKDIR /app

RUN git clone https://github.com/PlatinumCD/cperfmon.git && \
    cd cperfmon && \
    make

#ENV PERFMON_ENABLE_ALL_FIELDS=1
#ENV PERFMON_PUBLISH_DATA=1
#ENV PERFMON_NUMBER_SAMPLES=200
#ENV PERFMON_SAMPLE_INTERVAL=0.1
#ENV PERFMON_SIN_DATA_COLLECT=1

ENTRYPOINT ["/app/cperfmon/perfmon"]
