#include <stdio.h>
#include <stdlib.h>

/* check if all files are exhausted */
int scan(int *if_check, int file_cnt)
{
    int i;

	for (i = 0; i < file_cnt; i++)
        if (if_check[i])
            return 1;

    return 0;
}

/* find the index of the file with the not printed minimum value */
int minimum(int *if_check, long int *number, int file_cnt)
{
    int min = 0, i, j;

	for (i = 0; i < file_cnt; i++) {
		if (if_check[i]) {
			min = i;
			break;
		}
	}

	for (j = 0; j < file_cnt; j++)
		if ((if_check[j]) && (number[j] < number[min]))
			min = j;

	return min;
}

int main(int argc, char **argv)
{
	FILE **files;
	long int *number;
	int *if_check; /* 0 - checked, 1 - not cheked */
	int file_cnt = argc - 1, i, min, num, size;

    /* no files */
	if (file_cnt == 0) {
		putchar('\n');
		return 0;
	}

	files = (FILE **) malloc(file_cnt * sizeof(FILE *));
	number = (long int *) malloc(file_cnt * sizeof(long int));
	if_check = (int *) malloc(file_cnt * sizeof(int));

	for (i = 0; i < file_cnt; i++) {
		files[i] = fopen(argv[i + 1], "r");
		/* empty file */
        if_check[i] = !feof(files[i]);
        if (files[i]) {
			fseek(files[i], 0, SEEK_END);
			size = ftell(files[i]);
			rewind(files[i]);
			if (size == 0)
				if_check[i] = 0;
		}
        /* read the first values of non-empty files */
		if (if_check[i])
			fscanf(files[i], "%ld", &number[i]);
	}

	/* print numbers in non-descending order */
	while (scan(if_check, file_cnt)) {
		min = minimum(if_check, number, file_cnt);
		printf("%ld ", number[min]);
		if_check[min] = !feof(files[min]);
		if (if_check[min]) {
			num = fscanf(files[min], "%ld", &number[min]);
			if (num == EOF)
				if_check[min] = 0;
		}
	}
	putchar('\n');
    
	for (i = 0; i < file_cnt; i++)
		fclose(files[i]);
	free(files);
	free(number);
	free(if_check);

	return 0;
}