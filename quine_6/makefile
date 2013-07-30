# John Whelchel
# Makefile for quine

# Macros              

CC = clang -Wall -pedantic -std=c99
# CC = gcc -Wall -pedantic -std=c99

# CCFLAGS =                                                       
# CCFLAGS = -g
# CCFLAGS = -DNDEBUG                                                
# CCFLAGS = -DNDEBUG -O3
# CCFLAGS = -DNDEBUG -O3 -pg

TARGET = quine
SOURCES = quine.c
OBJECTS = quine.o

all: $(TARGET)

cleano:
	rm -f *.o;

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) $(OBJECTS) -o $(TARGET);
	
$(OBJECTS): $(SOURCES)
	$(CC) $(CCFLAGS) -c $(SOURCES);