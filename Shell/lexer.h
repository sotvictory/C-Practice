#ifndef LEXER_H
#define LEXER_H

typedef char **list;
typedef char *lexeme;

void print_list(list *lst, int *size_lst);
void clear_list(list *lst, int *size_lst, int *cur_lst);
void build_list(list *lst, int *size_lst);

#endif /* LEXER_H */