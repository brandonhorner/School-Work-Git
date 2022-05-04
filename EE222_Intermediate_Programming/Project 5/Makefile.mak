CC = gcc
CFLAGS = -Wall -Wextra -std=c11

sort : sort.o linked_list.o

linked_list.o : linked_list.c linked_list.h

sort.o : sort.c linked_list.h

.PHONY : clean debug
clean:
		rm -f sort *.o
		
debug: CFLAGS
debug: clean
debug: sort