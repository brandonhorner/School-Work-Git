CC = gcc
CFLAGS = -O0 -Wall -Wextra -std=c99

bubble_sort : bubble_sort.o

bubble_sort.o: bubble_sort.c

.PHONY: clean
clean:
	rm -f bubble_sort *.o