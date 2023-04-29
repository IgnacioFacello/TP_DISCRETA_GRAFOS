CC = gcc
CFLAGS = -Wall -O3 -DNDEBUG -pedantic -std=c99 -g
SOURCES =  main.c  EstructuraGrafo/EstructuraGrafo23.c
API_P1 = API_P1/APIG23.c API_P1/abb_vertex/abb.c
API_P2 = API_P2/APIParte2.c API_P2/tuplas/tuple.c API_P2/bitmap/bitmap.c API_P2/color_group/color_group.c API_P2/color_group/cg_list.c
OBJECTS = $(SOURCES:.c=.o) $(API_P1:.c=.o) $(API_P2:.c=.o)
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
	(for file in Grafos/* ; do (echo "\n$$file" ; date; time ./greedy < "$$file") ; done) > tests/test_all.txt 2>&1