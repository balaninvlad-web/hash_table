#include "hash_table.h"

static LinkedList* GetBucket (const HashTable* hash_table, const char* key)
{
    uint64_t hash = hash_table->hash_func (key);
    size_t index = hash % hash_table->size;
    return hash_table->buckets[index];
}

HashTable* HashCtor (size_t table_size, HashFunc hash_func)
{
    assert (table_size > 0);
    assert (hash_func);

    HashTable* hash_table = (HashTable*) calloc (1, sizeof(HashTable));
    if (!hash_table) 
    {
        fprintf (stderr, "HashCtor: failed to allocate HashTable\n");
        return NULL;
    }

    hash_table->buckets = (LinkedList**) calloc (table_size, sizeof(LinkedList*));

    if (!hash_table->buckets)
    {
        printf("HashCtor calloc ERROR\n");
        return NULL;
    }

    hash_table->size = table_size;
    hash_table->hash_func = hash_func;
    hash_table->load_factor = 0.0;

    for (size_t i = 0; i < table_size; i++)
    {
        hash_table->buckets[i] = ListCtor ();
        if (!hash_table->buckets[i])
        {
            for (size_t j = 0; j < i; j++) ListDtor (hash_table->buckets[j]);
            
            free (hash_table->buckets);
            free (hash_table);
            fprintf (stderr, "HashCtor calloc ERROR on %d element\n", i);
            return NULL;
        }
    }
    return hash_table;
}

void HashDtor (HashTable* hash_table)
{
    if (!hash_table) return;
    for (size_t i = 0; i < hash_table->size; ++i) 
    {
        if (hash_table->buckets[i]) ListDtor (hash_table->buckets[i]);
    }
    free (hash_table->buckets);
    free (hash_table);
}

void HashInsert (HashTable* hash_table, const char* key)
{
    if (!hash_table || !key) return;
    LinkedList* list = GetBucket (hash_table, key);
    Node* node = ListFindNode (list, key);
    if (node) node->count++;
    else InsertAfterNode (list, NULL, key);
}

int HashGet (const HashTable* hash_table, const char* key)
{
    if (!hash_table || !key) return 0;
    LinkedList* list = GetBucket (hash_table, key);
    Node* node = ListFindNode (list, key);
    return node ? node->count : 0;
}

void HashPrintStats(const HashTable* hash_table)
{
    if (!hash_table) return;
    size_t unique_words = 0;
    size_t max_chain = 0;

    for (size_t i = 0; i < hash_table->size; ++i) 
    {
        size_t chain_len = hash_table->buckets[i]->size;
        unique_words += chain_len;
        if (chain_len > max_chain) max_chain = chain_len;
    }

    double avg = (double) unique_words / hash_table->size;
    printf ("Hash table stats:\n");
    printf ("  Buckets: %zu\n", hash_table->size);
    printf ("  Unique words: %zu\n", unique_words);
    printf ("  Max chain length: %zu\n", max_chain);
    printf ("  Average chain length: %.2f\n", avg);
}