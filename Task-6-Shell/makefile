CC = gcc
CFLAGS = -Wall

all: shell

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c parser.h lexer.h
	$(CC) $(CFLAGS) -c parser.c

interpreter.o: interpreter.c interpreter.h
	$(CC) $(CFLAGS) -c interpreter.c

main.o: main.c lexer.h parser.h
	$(CC) $(CFLAGS) -c main.c

shell: lexer.o parser.o interpreter.o main.o
	$(CC) $(CFLAGS) lexer.o parser.o interpreter.o main.o -o shell

clean:
	rm -f lexer.o parser.o interpreter.o main.o shell