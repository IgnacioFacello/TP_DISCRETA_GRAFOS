CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g
SOURCES = abb.c test.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = readtree

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)