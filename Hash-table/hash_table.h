#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_SIZE 100

typedef struct {
    char* key;
    int value;
} item;

typedef struct {
    int size;
    int count;
    item** items;
} hash_table;

hash_table* create_table(int size);
void insert(hash_table* table, const char* key, int value);
int search(hash_table* table, const char* key);
void delete(hash_table* table, const char* key);
void destroy_table(hash_table* table);

#endif