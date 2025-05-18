CC = gcc
CFLAGS = -Wall -Iinclude -lpthread 
SRCDIR = src
INCDIR = include
BINDIR = bin
LOGDIR = log
CONFIGDIR = config

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = $(BINDIR)/client

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

