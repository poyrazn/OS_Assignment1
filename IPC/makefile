CC=gcc
CFLAGS=-I.
LFLAG=-lrt
OS := $(shell uname)
DEPS = ipc.h
OBJ = ipc.o

%.o: %.c $(DEPS)
ifeq ($(OS), Linux)
	$(CC) -c -o $@ $< $(CFLAGS) $(LFLAG)
else
	$(CC) -c -o $@ $< $(CFLAGS)
endif

Manager: $(OBJ)
ifeq ($(OS), Linux)
	gcc -o $@ $^ $(CFLAGS) $(LFLAG)
else
	gcc -o $@ $^ $(CFLAGS)
endif

.PHONY: clean

clean:
	rm -f *.o Manager
