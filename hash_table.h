#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <nmmintrin.h>
#include <x86intrin.h> 
#include "list_on_signs_func.h"

#ifndef AVX_STRCMP_H
#define AVX_STRCMP_H
#ifdef __cplusplus
extern "C" 
{
#endif
int MyAvxStrcmp (const char* a, const char* b);
#ifdef __cplusplus
}
#endif
#endif

typedef uint64_t (*HashFunc)(const char* key);

typedef struct 
{
    LinkedList** buckets;  
    size_t size; 
    float load_factor;    
    HashFunc hash_func;      
    //size_t count;  количество ключей пока нету        
} HashTable;

typedef struct 
{
    const char* name;
    HashFunc func;
} HashFuncInfo;

HashTable* HashCtor (size_t table_size, HashFunc hash_func);
void HashDtor (HashTable* hash_table);
void HashInsert (HashTable* hash_table, const char* key);
int HashGet (const HashTable* hash_table, const char* key);
void HashPrintStats (const HashTable* hash_table);
void HashTablePrintBuckets (const HashTable* hash_table);
void HashTableDescription (const HashTable* hash_table, const char* filename);
void CompletelyHashFuncs (HashTable* hash_table, char** words, int* word_count, const char* filename);
size_t SearchWordsHashTable (HashTable* hash_table, char** words, int test_word_count, int repetitions);
#endif