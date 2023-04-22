CC = gcc
CFLAGS = -Wall -Werror -O3 -DNDEBUG -pedantic -std=c99 -g
SOURCES = API_P2/APIParte2.c API_P1/APIG23.c main.c API_P1/abb_vertex/abb.c EstructuraGrafo/EstructuraGrafo23.c API_P2/tuple.c API_P2/terna.c API_P2/bitmap.c
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

testall: $(TARGET)
	(for file in Grafos/* ; do (echo "\n$$file" ; time ./greedy < "$$file") ; done) > tests/test_all.txt