CC=gcc
CFLAGS=-g -Wall

all: 1_sum 2_convert_and_sort 3_create_binary_file 3_set_diagonal_bits 5_directory

1_sum:
	$(CC) $(CFLAGS) 1_sum.c -o 1_sum

2_convert_and_sort:
	$(CC) $(CFLAGS) 2_convert_and_sort.c -o 2_convert_and_sort

3_create_binary_file:
	$(CC) $(CFLAGS) 3_create_binary_file.c -o 3_create_binary_file

3_set_diagonal_bits:
	$(CC) $(CFLAGS) -lm 3_set_diagonal_bits.c -o 3_set_diagonal_bits

5_directory:
	$(CC) $(CFLAGS) 5_directory.c -o 5_directory

clean:
	rm -f *.o 1_sum 2_convert_and_sort 3_create_binary_file 3_set_diagonal_bits 5_directory