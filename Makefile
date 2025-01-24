CC      = gcc
CFLAGS  = -Wall -Wextra -O3

SRC = \
  src/main.c \
  src/monitor_config.c \
  src/monitor_utils.c \
  src/cpu_frame.c \
  src/cpu_monitor.c \
  src/cpu_output_csv.c \
  src/cpu_output_plugin.c

OBJ = $(SRC:.c=.o)

BINARY_NAME = perfmon

all: $(BINARY_NAME)

$(BINARY_NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lm -lwaggle

%.o: %.c
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	rm -f $(OBJ) $(BINARY_NAME)