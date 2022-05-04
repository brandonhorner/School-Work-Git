CC = gcc 
CFLAGS = -O0 -Wall -Wextra -pedantic
LDFLAGS = -lm
LDLIBS = -lm

basic_io : basic_io.o

basic_io.o : basic_io.c

.PHONY: clean
clean :
	rm -f basic_io *.o
