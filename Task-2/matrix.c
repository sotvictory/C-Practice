#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

void mem_err(void)
{
    fprintf(stderr, "OS did not give memory. Exit.\n");
    exit(1);
}

matrix **read_matrix(int row_cnt, int col_cnt)
{
    matrix **mas;
    int i, j;

    mas = (matrix **) malloc(sizeof(matrix *) * row_cnt);
    if (mas == NULL)
        mem_err();

    for (i = 0; i < row_cnt; i++) {
        mas[i] = (matrix *) malloc(sizeof(matrix) * col_cnt);
        if (mas[i] == NULL)
            mem_err();
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

matrix **sum_matrix(matrix **mas_1, int row_cnt_1, int col_cnt_1, matrix **mas_2, int row_cnt_2, int col_cnt_2)
{
    matrix **mas;
    int i, j;

    if (row_cnt_1 != row_cnt_2 || col_cnt_1 != col_cnt_2) {
        fprintf(stderr, "Cannot calculate the sum of matrices: the dimensions do not match\n");
        exit(2);
    }

    mas = (matrix **) malloc(sizeof(matrix *) * row_cnt_1);
    if (mas == NULL)
        mem_err();

    for (i = 0; i < row_cnt_1; i++) {
        mas[i] = (matrix *) malloc(sizeof(matrix) * col_cnt_1);
        if (mas[i] == NULL)
            mem_err();
        for (j = 0; j < col_cnt_1; j++) {
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
    if (mas_scale == NULL)
        mem_err();

    for (i = 0; i < row_cnt; i++) {
        mas_scale[i] = (matrix *) malloc(sizeof(matrix) * col_cnt);
        if (mas_scale == NULL)
            mem_err();
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

    if (row_cnt_1 != row_cnt_2 || col_cnt_1 != col_cnt_2) {
        fprintf(stderr, "Cannot calculate the product of matrices: the dimensions do not match\n");
        exit(2);       
    }

    mas = (matrix **) malloc(sizeof(matrix *) * row_cnt_1);
    if (mas == NULL)
        mem_err();

    for (i = 0; i < row_cnt_1; i++) {
        mas[i] = (matrix *) malloc(sizeof(matrix) * col_cnt_2);
        if (mas[i] == NULL)
            mem_err();
        for (j = 0; j < col_cnt_2; j++) {
            mas[i][j] = 0;
            for (k = 0; k < col_cnt_1; k++) {
                mas[i][j] += mas_1[i][k] * mas_2[k][j];
            }
        }
    }

    return mas;
}

matrix** triang_matrix(matrix **mas, int col_cnt)
{
    matrix **mas_triangle;
    int i, j, k;
    double lead_elem;

    mas_triangle = (matrix **) malloc(sizeof(matrix *) * col_cnt);
    if (mas_triangle == NULL)
        mem_err();

    for (i = 0; i < col_cnt; i++) {
        mas_triangle[i] = (matrix *) malloc(sizeof(matrix) * col_cnt);
        if (mas_triangle[i] == NULL)
            mem_err();
        memcpy(mas_triangle[i], mas[i], sizeof(matrix) * col_cnt);
    }

    for (k = 0; k < col_cnt - 1; k++) {
        for (i = k + 1; i < col_cnt; i++) {
            lead_elem = mas_triangle[i][k] / mas_triangle[k][k];
            for (j = k; j < col_cnt; j++) {
                mas_triangle[i][j] -= lead_elem * mas_triangle[k][j];
            }
        }
    }

    return mas_triangle;
}

double det_matrix(matrix **mas, int row_cnt, int col_cnt)
{
    matrix **mas_triangle;
    int i;
    double det = 1;

    mas_triangle = triang_matrix(mas, col_cnt);
    for (i = 0; i <  col_cnt; i++)
        det *= mas_triangle[i][i];

    free_matrix(mas_triangle, row_cnt);

    return det;
}