CC = gcc
CFLAGS = -O0 -Wall -Wextra -std=c99

linked_list : tests.o linked_list.o

linked_list.o : linked_list.c linked_list.h

tests.o : linked_list.h linked_list.c

.PHONY : clean
clean:
		rm - f linked_list *.o