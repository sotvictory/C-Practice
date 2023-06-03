#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

unsigned int hash(const char* key) {
    unsigned int hash_value = 0;
    for (unsigned int i = 0; i < strlen(key); i++) {
        hash_value = key[i] + (hash_value << 5) - hash_value;
    }
    return hash_value % TABLE_SIZE;
}

hash_table* create_table(int size) {
    hash_table* table = (hash_table*) calloc(1, sizeof(hash_table));
    table->size = size;
    table->count = 0;
    table->items = (item**) calloc(table->size, sizeof(item*));
    return table;
}

void insert(hash_table* table, const char* key, int value) {
    unsigned int index = hash(key) % table->size;
    item* new_item = (item*) malloc(sizeof(item));
    new_item->key = (char*) malloc(strlen(key) + 1);
    strcpy(new_item->key, key);
    new_item->value = value;
    table->items[index] = new_item;
    table->count++;
}

int search(hash_table* table, const char* key) {
    unsigned int index = hash(key) % table->size;
    if (table->items[index] != NULL && strcmp(table->items[index]->key, key) == 0) {
        return table->items[index]->value;
    } else {
        return -1;
    }
}

void delete(hash_table* table, const char* key) {
    unsigned int index = hash(key) % table->size;
    if (table->items[index] != NULL) {
        free(table->items[index]->key);
        free(table->items[index]);
        table->items[index] = NULL;
        table->count--;
    }
}

void destroy_table(hash_table* table) {
    for (int i = 0; i < table->size; i++) {
        if (table->items[i] != NULL) {
            free(table->items[i]->key);
            free(table->items[i]);
        }
    }
    free(table->items);
    free(table);
}