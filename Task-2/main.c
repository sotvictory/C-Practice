#include <stdio.h>
#include "matrix.h"

enum { 
    row_cnt = 2,
    col_cnt = 3
};

int main(void)
{
    matrix **mas = read_matrix(row_cnt, col_cnt);
    print_matrix(mas, row_cnt, col_cnt);
    free_matrix(mas, row_cnt);

    return 0;
}