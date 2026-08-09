CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Add source files as they are implemented by the team
# E.g., SRCS = main.c queue.c input.c fcfs.c srtf.c priority.c timeline.c metrics.c display.c process_manager.c
SRCS = main.c queue.c

OBJS = $(SRCS:.c=.o)
TARGET = os_project

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
