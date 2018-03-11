CC=gcc
CFLAGS=-I.

DEPS = assignment1.h
OBJ = assignment1.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Assignment1: $(OBJ)
	     gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o