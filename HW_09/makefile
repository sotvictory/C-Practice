CC = gcc
CFLAGS = -Wall -Werror

all: ioredirect subshell cmd conveyor str_search

redirect: ioredirect.c
	$(CC) $(CFLAGS) ioredirect.c -o ioredirect

subshell: subshell.c
	$(CC) $(CFLAGS) subshell.c -o subshell

cmd: cmd.c
	$(CC) $(CFLAGS) cmd.c -o cmd

conveyor: conveyor.c
	$(CC) $(CFLAGS) conveyor.c -o conveyor

str_search: str_search.c
	$(CC) $(CFLAGS) str_search.c -o str_search

clean:
	rm -f ioredirect subshell cmd conveyor str_search