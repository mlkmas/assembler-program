CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -Iinclude
TARGET = timediff
SRCS = src/timediff.c src/time.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
