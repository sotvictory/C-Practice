#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

matrix **read_matrix(int row_cnt, int col_cnt)
{
    matrix **mas;
    int i, j;

    mas = (matrix **) malloc(sizeof(matrix *) * row_cnt);

    for (i = 0; i < row_cnt; i++) {
        mas[i] = (matrix *) malloc(sizeof(matrix) * col_cnt);
        for (j = 0; j < col_cnt; j++) {
            scanf("%lf", &mas[i][j]);
        }
    }

    return mas;
}

void print_matrix(matrix **mas, int row_cnt, int col_cnt) 
{
    int i, j;

    for (i = 0; i < row_cnt; i++) {
        for (j = 0; j < col_cnt; j++) {
            printf("%7.3lf ", mas[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(matrix **mas, int row_cnt)
{
    int i;

    for (i = 0; i < row_cnt; i++) {
        free(mas[i]);
    }
    free(mas);
}

matrix **sum_matrix(matrix **mas_1, matrix **mas_2, int row_cnt, int col_cnt)
{
    matrix **mas;
    int i, j;

    mas = (matrix **) malloc(sizeof(matrix *) * row_cnt);
    for (i = 0; i < row_cnt; i++) {
        mas[i] = (matrix *) malloc(sizeof(matrix) * col_cnt);
        for (j = 0; j < col_cnt; j++) {
            mas[i][j] = mas_1[i][j] + mas_2[i][j];
        }
    }

    return mas;
}

matrix **scalar_prod_matrix(double multiplier, matrix **mas, int row_cnt, int col_cnt)
{
    matrix **mas_scalar;
    int i, j;

    mas_scalar = (matrix **) malloc(sizeof(matrix *) * row_cnt);
    for (i = 0; i < row_cnt; i++) {
        mas_scalar[i] = (matrix *) malloc(sizeof(matrix) * col_cnt);
        for (j = 0; j < col_cnt; j++) {
            mas_scalar[i][j] = multiplier * mas[i][j];
        }
    }

    return mas_scalar;
}