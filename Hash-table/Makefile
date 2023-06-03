CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99
OBJS = main.o hash_table.o

all: hash_table

hash_table: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o hash_table

main.o: main.c hash_table.h
	$(CC) $(CFLAGS) -c main.c

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c hash_table.c

clean:
	rm -f $(OBJS) hash_table