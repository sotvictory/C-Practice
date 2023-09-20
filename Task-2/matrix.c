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

matrix **scaling_matrix(double multiplier, matrix **mas, int row_cnt, int col_cnt)
{
    matrix **mas_scale;
    int i, j;

    mas_scale = (matrix **) malloc(sizeof(matrix *) * row_cnt);
    for (i = 0; i < row_cnt; i++) {
        mas_scale[i] = (matrix *) malloc(sizeof(matrix) * col_cnt);
        for (j = 0; j < col_cnt; j++) {
            mas_scale[i][j] = multiplier * mas[i][j];
        }
    }

    return mas_scale;
}

matrix **product_matrix(matrix **mas_1, int row_cnt_1, int col_cnt_1, matrix **mas_2, int row_cnt_2, int col_cnt_2)
{
    matrix **mas;
    int i, j, k;

    mas = (matrix **) malloc(sizeof(matrix *) * row_cnt_1);
    for (i = 0; i < row_cnt_1; i++) {
        mas[i] = (matrix *) malloc(sizeof(matrix) * col_cnt_2);
        for (j = 0; j < col_cnt_2; j++) {
            mas[i][j] = 0;
            for (k = 0; k < col_cnt_1; k++) {
                mas[i][j] += mas_1[i][k] * mas_2[k][j];
            }
        }
    }

    return mas;
}