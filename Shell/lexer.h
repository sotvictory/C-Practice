#ifndef LEXER_H
#define LEXER_H

typedef char **list;
typedef char *lexeme;

void print_list(list lst, int size_lst, int output_fd);
list clear_list(list *lst, int *size_lst);
list build_list(list *lst, int *size_lst, int input_fd, int output_fd);

#endif /* LEXER_H */