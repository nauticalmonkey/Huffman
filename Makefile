CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
EMAIN = hencode
DMAIN = hdecode
EOBJ = hencode.o Functions.o
DOBJ = hdecode.o Functions.o 


all : $(EMAIN) $(DMAIN)

$(EMAIN) : $(EOBJ) hencode.h
	$(CC) $(CFLAGS) -o $(EMAIN) $(EOBJ)

$(DMAIN) : $(DOBJ) hdecode.h
	$(CC) $(CFLAGS) -o $(DMAIN) $(DOBJ)

hencode.o : hencode.c hencode.h
	$(CC) $(CFLAGS) -c hencode.c

hdecode.o : hdecode.c hdecode.h
	$(CC) $(CFLAGS) -c hdecode.c

Functions.o : Functions.c Functions.h
	$(CC) $(CFLAGS) -c Functions.c

clean :
	rm *.o $(MAIN) core

