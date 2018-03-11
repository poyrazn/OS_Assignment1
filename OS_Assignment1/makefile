CC=gcc
CFLAGS=-I.
LFLAG=-lrt
OS := $(shell uname)
DEPS = assignment1.h
OBJ = assignment1.o

%.o: %.c $(DEPS)
ifeq ($(OS), Linux)
	$(CC) -c -o $@ $< $(CFLAGS) $(LFLAG)
else
	$(CC) -c -o $@ $< $(CFLAGS)
endif

Assignment1: $(OBJ)
ifeq ($(OS), Linux)
	gcc -o $@ $^ $(CFLAGS) $(LFLAG)
else
	gcc -o $@ $^ $(CFLAGS)
endif

.PHONY: clean

clean:
	rm -f *.o Assignment1
