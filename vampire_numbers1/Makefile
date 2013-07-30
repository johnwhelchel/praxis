# John Whelchel
# Makefile for vampire.c
# 7/2//13

# Macros              

CC = clang -std=c99 -Wall -pedantic
#CC = gcc -std=c99 -Wall -pedantic
													 
 CCFLAGS =                                                       
# CCFLAGS = -g
# CCFLAGS = -DNDEBUG                                                
# CCFLAGS = -DNDEBUG -O3
# CCFLAGS = -DNDEBUG -O3 -pg


all: supervampire vampire 

cleano:
	rm -f *.o;

supervampire: supervampire.o
	$(CC) $(CCFLAGS) supervampire.o -o supervampire;

vampire: vampire.o	
	$(CC) $(CCFLAGS) vampire.o -o vampire;

supervampire.o: supervampire.c
	$(CC) $(CCFLAGS) -c supervampire.c;
	
vampire.o: vampire.c
	$(CC) $(CCFLAGS) -c vampire.c;