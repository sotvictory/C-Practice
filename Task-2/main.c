#include <stdio.h>
#include "matrix.h"

enum { 
    row_cnt_1 = 3,
    col_cnt_1 = 3,
    row_cnt_2 = 3,
    col_cnt_2 = 3,
};

const double multiplier = 2.0;

int main(void)
{
    matrix **mas_1, **mas_2, **mas_sum, **mas_scale, **mas_prod;
    double det;
    
    mas_1 = read_matrix(row_cnt_1, col_cnt_1);
    mas_2 = read_matrix(row_cnt_2, col_cnt_2);

    printf("Matrix 1:\n");
    print_matrix(mas_1, row_cnt_1, col_cnt_1);
    printf("Matrix 2:\n");
    print_matrix(mas_2, row_cnt_2, col_cnt_2);

    mas_scale = scaling_matrix(multiplier, mas_1, row_cnt_1, col_cnt_1);
    printf("Product of multiplier %.3lf and matrix 1:\n", multiplier);
    print_matrix(mas_scale, row_cnt_1, col_cnt_1);

    det = det_matrix(mas_1, row_cnt_1, col_cnt_1);
    printf("Determinant of matrix 1: %.3lf\n", det);

    mas_prod = product_matrix(mas_1, row_cnt_1, col_cnt_1, mas_2, row_cnt_2, col_cnt_2);
    printf("Product of matrix 1 and matrix 2:\n");
    print_matrix(mas_prod, row_cnt_1, col_cnt_2);

    mas_sum = sum_matrix(mas_1, row_cnt_1, col_cnt_1, mas_2, row_cnt_2, col_cnt_2);
    printf("Sum of matrix 1 and matrix 2:\n");
    print_matrix(mas_sum, row_cnt_1, col_cnt_1);

    free_matrix(mas_1, row_cnt_1);
    free_matrix(mas_2, row_cnt_2);
    free_matrix(mas_sum, row_cnt_1);
    free_matrix(mas_scale, row_cnt_1);
    free_matrix(mas_prod, row_cnt_1);

    return 0;
}