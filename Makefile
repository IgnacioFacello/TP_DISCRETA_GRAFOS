CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g
SOURCES = APIG23.c main.c binary_search_trees_lib/abb.c lib_grafos/EstructuraGrafo23.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = reader

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)
