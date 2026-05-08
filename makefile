CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -Iinclude
TARGET = assembler
SRCS = src/assembler.c src/preAssembler.c src/firstPass.c \
       src/secondPass.c src/tables.c src/instructions.c \
       src/utils.c src/errors.c src/codeConversion.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
