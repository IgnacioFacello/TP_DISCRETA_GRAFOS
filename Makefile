CC = gcc
CFLAGS = -Wall -O3 -DNDEBUG -pedantic -std=c99 -g
SOURCES = APIParte2.c APIG23.c main.c abb.c EstructuraGrafo23.c abbU32.c tuple.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = greedy

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)

test: $(TARGET)
	(for file in Grafos/* ; do (echo "$$file" ; ./greedy < "$$file") ; done) > tests/test.txt