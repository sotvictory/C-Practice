#include <stdio.h>
#include <stdlib.h>

int scan(int *scanned, int filecounter)
{
    int i;

	for (i = 0; i < filecounter; i++)
        if (scanned[i])
            return 1;

    return 0;
}

int minimum(int *scanned, long int *number, int filecounter)
{
    int min = 0, i, j;

	for (i = 0; i < filecounter; i++) {
		if (scanned[i]) {
			min = i;
			break;
		}
	}
	for (j = 0; j < filecounter; j++)
		if ((scanned[j]) && (number[j] < number[min]))
			min = j;

	return min;
}

int main(int argc, char **argv)
{
	FILE **files;
	long int *number;
	int *scanned; // 0 - файл больше не просматриваем, 1 - продолжаем
	int filecounter = argc - 1, i, min, num, size;

    // нет файлов
	if (filecounter == 0) {
		printf("\n");
		return 0;
	}

    // если есть, то выделяем память
	files = (FILE **) malloc(filecounter * sizeof(FILE*));
	number = (long int *) malloc(filecounter * sizeof(long int));
	scanned = (int *) malloc(filecounter * sizeof(int));

    // проверки
	for (i = 0; i < filecounter; i++) {
        // открыли
		files[i] = fopen(argv[i + 1], "r");
        // могут быть пустые
        scanned[i] = !feof(files[i]);
        if (files[i]) {
            // проанализировали размер файла
			fseek(files[i], 0, SEEK_END);
			size = ftell(files[i]);
			rewind(files[i]);
			if (size == 0)
				scanned[i] = 0;
		}
        // считали первые значения у непустых файлов
		if (scanned[i])
			fscanf(files[i], "%ld", &number[i]);
	}

    // просматриваем и обрабатываем файлы
	while (scan(scanned, filecounter)) {
		min = minimum(scanned, number, filecounter);
		printf("%ld ", number[min]);
		scanned[min] = !feof(files[min]);
		if (scanned[min]) {
			num = fscanf(files[min], "%ld", &number[min]);
			if (num == EOF)
				scanned[min] = 0;
		}
	}
    
    // закрыли и освободили память
	for (i = 0; i < filecounter; i++)
		fclose(files[i]);
	free(files);
	free(number);
	free(scanned);
    printf("\n");

	return 0;
}