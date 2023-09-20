#include <stdio.h>
#include "matrix.h"

enum { 
    row_cnt = 2,
    col_cnt = 3,
};

const double multiplier = 2.0;

int main(void)
{
    matrix **mas_1, **mas_2, **mas_sum, **mas_scalar;
    
    mas_1 = read_matrix(row_cnt, col_cnt);
    mas_2 = read_matrix(row_cnt, col_cnt);

    printf("Matrix 1:\n");
    print_matrix(mas_1, row_cnt, col_cnt);
    printf("Matrix 2:\n");
    print_matrix(mas_2, row_cnt, col_cnt);

    mas_sum = sum_matrix(mas_1, mas_2, row_cnt, col_cnt);
    printf("Sum of matrix 1 and matrix 2:\n");
    print_matrix(mas_sum, row_cnt, col_cnt);

    mas_scalar = scalar_prod_matrix(multiplier, mas_1, row_cnt, col_cnt);
    printf("Product of multiplier and matrix 1:\n");
    print_matrix(mas_scalar, row_cnt, col_cnt);

    free_matrix(mas_1, row_cnt);
    free_matrix(mas_2, row_cnt);
    free_matrix(mas_sum, row_cnt);
    free_matrix(mas_scalar, row_cnt);

    return 0;
}