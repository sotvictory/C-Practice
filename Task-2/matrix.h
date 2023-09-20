#ifndef MATRIX_H
#define MATRIX_H

typedef double matrix;

matrix **read_matrix(int row_cnt, int col_cnt);
void free_matrix(matrix **mas, int row_cnt);
void print_matrix(matrix **mas, int  row_cnt, int  col_cnt);

matrix **sum(matrix **mas_1, matrix **mas_2, int row_cnt, int col_cnt);
matrix **scalar_prod(double multiplier, matrix **mas, int row_cnt, int col_cnt);
matrix **matrix_product(matrix **mas_1, int row_cnt_1, int col_cnt_1, matrix **mas_2, int row_cnt_2, int col_cnt_2);
double det(matrix** mas, int row_cnt, int col_cnt);

#endif /* MATRIX_H */ 