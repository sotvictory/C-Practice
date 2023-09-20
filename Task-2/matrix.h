#ifndef MATRIX_H
#define MATRIX_H

typedef double matrix;

matrix **read_matrix(int row_cnt, int col_cnt);
void free_matrix(matrix **mas, int row_cnt);
void print_matrix(matrix **mas, int  row_cnt, int  col_cnt);

matrix **sum_matrix(matrix **mas_1, matrix **mas_2, int row_cnt, int col_cnt);
matrix **scaling_matrix(double multiplier, matrix **mas, int row_cnt, int col_cnt);
matrix **product_matrix(matrix **mas_1, int row_cnt_1, int col_cnt_1, matrix **mas_2, int row_cnt_2, int col_cnt_2);
double det_matrix(matrix** mas, int row_cnt, int col_cnt);

#endif /* MATRIX_H */ 