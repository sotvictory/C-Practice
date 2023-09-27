#include <stdio.h>
#include <stdlib.h>

#define M 50
#define N 20

void print_row(double *row, int col_cnt)
{
    int i;

    for (i = 0; i < col_cnt; i++)
        printf("%7.3lf ", row[i]);
    printf("\n");
}

/* The function finds the matrix row with greatest number of negative elements 
 * and prints all the elements of this row. If there are several rows, then prints all these rows*/

void max_neg_rows(double **matrix)
{
    int max_cnt = 0, i, j;
    int neg_cnt[M] = {0};

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            if (matrix[i][j] < 0)
                neg_cnt[i]++;
        }
        if (neg_cnt[i] > max_cnt)
            max_cnt = neg_cnt[i];
    }

    if (max_cnt == 0) {
        printf("There are no rows with negative elements\n");
        return;
    }

    printf("Matrix rows with maximum number of negative elements:\n");
    for (i = 0; i < M; i++) {
        if (neg_cnt[i] == max_cnt)
            print_row(matrix[i], N);
    }
}

void mem_err(void)
{
    fprintf(stderr, "OS did not give memory. Exit.\n");
    exit(1);
}

double **read_matrix(void)
{
    double **mas;
    int i, j;

    mas = (double **) malloc(sizeof(double *) * M);
    if (mas == NULL)
        mem_err();

    for (i = 0; i < M; i++) {
        mas[i] = (double *) malloc(sizeof(double) * N);
        if (mas[i] == NULL)
            mem_err();
        for (j = 0; j < N; j++) {
            scanf("%lf", &mas[i][j]);
        }
    }

    return mas;
}

void free_matrix(double **mas)
{
    int i;

    for (i = 0; i < M; i++) {
        free(mas[i]);
    }
    free(mas);
}

int main(void)
{
    double **matrix = read_matrix();
    max_neg_rows(matrix);
    free_matrix(matrix);

    return 0;
}