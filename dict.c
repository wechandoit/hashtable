#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DICT_CAPACITY 50000

// simple hash function, change to something more FIPS140 complient
// sha256 is mostly collision-proof as cases of such have not been found
long hash_func(char* str)
{
    long i = 0;

    for (int j = 0; str[j]; j++) i+= str[j];

    return i % DICT_CAPACITY;
}

typedef struct hash_item
{
    char* key;
    char* value;
} hash_item;

typedef struct hashtable
{
    hash_item** items;
    int size;
    int count;
} hashtable;

hash_item* createHashItem(char* key, char* value)
{
    hash_item* item = (hash_item*) malloc(sizeof(hash_item));
    item->key = (char*) malloc(strlen(key) + 1);
    item->value = (char*) malloc(strlen(value) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

void freeHashItem(hash_item* item)
{
    if (item != NULL)
    {
        free(item->key);
        free(item->value);
        free(item);
    }
}

hashtable* createHashTable(int size)
{
    hashtable* table = (hashtable*) malloc(sizeof(hashtable));
    table->size = size;
    table->count = 0;
    table->items = (hash_item **) calloc(table->size, sizeof(hash_item*));

    for (int i = 0; i < table->size; i++) table->items[i] = NULL;

    return table;
}

void freeHashTable(hashtable* table)
{
    for (int i = 0; i < table->size; i++)
    {
        hash_item* item = table->items[i];
        if (item != NULL) freeHashItem(item);
    }

    free(table->items);
    free(table);
}

void printHashTable(hashtable* table)
{
    printf("HASH TABLE:\n");
    for (int i = 0; i < table->size; i++)
    {
        if (table->items[i]) printf("INDEX:%d | KEY:%s | VALUE:%s\n", i, table->items[i]->key, table->items[i]->value);
    }
}

int hashtableContains(hashtable* table, char* key)
{
    int index = hash_func(key);
    hash_item* currentItem = table->items[index];
    return currentItem != NULL;
}

void handleHashCollision(hashtable* table, hash_item* item)
{

}

void hashtableInsert(hashtable* table, char* key, char* value)
{
    hash_item* item = createHashItem(key, value);
    int index = hash_func(key);

    hash_item* currentItem = table->items[index];
    if (currentItem == NULL)
    {
        if (table->count == table->size)
        {
            // table is full
            printf("TABLE IS FULL\n");
            freeHashItem(item);
            return;
        }

        table->items[index] = item;
        table->count++;
    }
    else
    {
        // update
        if (strcmp(currentItem->key, key) == 0)
        {
            strcpy(table->items[index]->value, value);
            return;
        }
        // handle collision if can't update
        else
        {
            handleHashCollision(table, item);
            return;
        }
    }
}



// testing
void main()
{
    hashtable* table = createHashTable(DICT_CAPACITY);

    char* str = "network.web";
    char* str2 = "network.yes";

    printf("STR1: %ld\n", hash_func(str));
    printf("STR2: %ld\n", hash_func(str2));

    hashtableInsert(table, str, "10481904");
    hashtableInsert(table, str2, "12784912749");

    printHashTable(table);

    freeHashTable(table);
}