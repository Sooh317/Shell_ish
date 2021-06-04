CC = gcc
CFLAGS = -Wall -O
SRCS = $(wildcard *.c)
SRCS += $(wildcard ./parser/*.c)
SRCS += $(wildcard ./codes/*.c)
OBJS = $(SRCS:.c=.o)

TARGET = ish

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) $(OBJS) *~
