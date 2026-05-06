#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include "list_on_signs_func.h"

typedef uint64_t (*HashFunc)(const char* key);

typedef struct 
{
    LinkedList** buckets;  
    size_t size; 
    float load_factor;    
    HashFunc hash_func;      
    //size_t count;  количество ключей пока нету        
} HashTable;

HashTable* HashCtor (size_t table_size, HashFunc hash_func);
void HashDtor (HashTable* hash_table);
void HashInsert (HashTable* hash_table, const char* key);
int HashGet (const HashTable* hash_table, const char* key);
void HashPrintStats (const HashTable* hash_table);
void HashTablePrintBuckets (const HashTable* hash_table);
#endif