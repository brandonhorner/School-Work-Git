CC = gcc
CFLAGS = -O0 -Wall -Wextra -std=c99

maze_runner : runner.o maze_runner.o mazelib.o

runner.o : runner.c mazelib.h

maze_runner.o : mazelib.h mazelib.c

.PHONY : clean
clean:
		rm - f runner *.o