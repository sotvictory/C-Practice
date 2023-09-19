line_lib.o: line_lib.c line_lib.h
	gcc -c -o line_lib.o line_lib.c
main.o: main.c line_lib.h
	gcc -c -o main.o main.c
task_3: main.o line_lib.o
	gcc -o task_3 main.o line_lib.o