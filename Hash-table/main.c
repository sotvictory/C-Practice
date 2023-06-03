#include <stdio.h>
#include "hash_table.h"

int main() 
{
    hash_table* table = create_table(TABLE_SIZE);

    insert(table, "key 1", 1);
    insert(table, "key 2", 2);
    insert(table, "key 3", 3);
    printf("Value of key 1: %d\n", search(table, "key 1"));
    printf("Value of key 2: %d\n", search(table, "key 2"));
    printf("Value of key 3: %d\n", search(table, "key 3"));
    printf("Now the table contains %d items\n", table->count);

    delete(table, "key 2");
    printf("Value of key 2 after deletion: %d\n", search(table, "key 2"));
    printf("Now the table contains %d items\n", table->count);

    destroy_table(table);

    return 0;
}
