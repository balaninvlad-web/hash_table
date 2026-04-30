#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

typedef struct 
{
    HashNode** buckets;  
    size_t size;          
    //size_t count;  количество ключей пока нету        
    unsigned int (*hash) (const char* key, size_t table_size);
} HashTable;

typedef struct
{
    char* key;
    int value;
    HashNode* next;
    HashNode* prev;
} HashNode;

HashTable* HashCreate (size_t table_size, HashFunc hash_func);
void HashInsert (HashTable* ht, const char* key);
int HashSearch (const HashTable* ht, const char* key);
void HashFree (HashTable* ht);

#endif