#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

unsigned int hash(const char *key) {
    unsigned int hash_value = 0;

    for (unsigned int i = 0; i < strlen(key); i++) {
        hash_value = key[i] + (hash_value << 5) - hash_value;
    }

    return hash_value % TABLE_SIZE;
}

HashTable * create_table(int size) {
    HashTable *table = calloc(1, sizeof(HashTable));

    table->size = size;
    table->count = 0;
    table->items = calloc(table->size, sizeof(Item*));

    return table;
}

void insert_item(HashTable *table, const char *key, int value) {
    unsigned int index = hash(key) % table->size;

    Item *new_item = calloc(1, sizeof(Item));
    new_item->key = calloc(strlen(key) + 1, sizeof(char));

    strcpy(new_item->key, key);
    new_item->value = value;
    table->items[index] = new_item;
    table->count++;
}

int search_item(HashTable *table, const char *key) {
    unsigned int index = hash(key) % table->size;

    if (table->items[index] != NULL && strcmp(table->items[index]->key, key) == 0) {
        return table->items[index]->value;
    } else {
        return -1;
    }
}

void delete_item(HashTable *table, const char *key) {
    unsigned int index = hash(key) % table->size;
    
    if (table->items[index] != NULL) {
        free(table->items[index]->key);
        free(table->items[index]);
        table->items[index] = NULL;
        table->count--;
    }
}

void destroy_table(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->items[i] != NULL) {
            free(table->items[i]->key);
            free(table->items[i]);
        }
    }
    free(table->items);
    free(table);
}