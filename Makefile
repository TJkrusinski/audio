# Variables
CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGETS = capture playback devices
CAPTURE_SRCS = capture.c
CAPTURE_OBJS = $(CAPTURE_SRCS:.c=.o)
PLAYBACK_SRCS = playback.c
PLAYBACK_OBJS = $(PLAYBACK_SRCS:.c=.o)
DEVICES_SRCS = devices.c
DEVICES_OBJS = $(DEVICES_SRCS:.c=.o)
DEPS = miniaudio.h

# Rules
all: $(TARGETS)

capture: $(CAPTURE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

playback: $(PLAYBACK_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

devices: $(DEVICES_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(CAPTURE_OBJS) $(PLAYBACK_OBJS) $(DEVICES_OBJS) $(TARGETS)
