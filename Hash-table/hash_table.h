#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED

enum { TABLE_SIZE = 100 };

typedef struct {
    char *key;
    int value;
} Item;

typedef struct {
    int size;
    int count;
    Item **items;
} HashTable;

HashTable * create_table(int size);
void insert_item(HashTable *table, const char *key, int value);
int search_item(HashTable *table, const char *key);
void delete_item(HashTable *table, const char *key);
void destroy_table(HashTable *table);

#endif
