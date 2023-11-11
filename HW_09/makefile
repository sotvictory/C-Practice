CC = gcc
CFLAGS = -Wall -Werror

all: ioredirect subshell 

redirect: ioredirect.c
	$(CC) $(CFLAGS) ioredirect.c -o ioredirect

subshell: subshell.c
	$(CC) $(CFLAGS) subshell.c -o subshell

clean:
	rm -f ioredirect subshell