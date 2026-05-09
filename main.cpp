#include "hash_table.h"
#include "read_file.h"
#include "list_on_signs_func.h"
#include "hash_funcs.h"

#define HASH_FUNC_ENTRY(func) { #func, func }

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf ("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    int word_count = 0;
    char** words = FullWordLoading (argv[1], &word_count);
    if (!words) return 1;
    
    int test_word_count = 0;
    char** test_words = FullWordLoading (argv[2], &test_word_count);
    if (!test_words) return 1;

    HashFuncInfo funcs[] = 
    {
        HASH_FUNC_ENTRY (HashAlwaysOne),
        HASH_FUNC_ENTRY (HashFirstChar),
        HASH_FUNC_ENTRY (HashLength),
        HASH_FUNC_ENTRY (HashSum),
        HASH_FUNC_ENTRY (HashRotateLeft),
        HASH_FUNC_ENTRY (HashRotateRight),
        HASH_FUNC_ENTRY (HashGnu),
        HASH_FUNC_ENTRY (HashCrc32)
    };
    size_t func_count = sizeof (funcs) / sizeof (funcs[0]);

    for (size_t i = 0; i < func_count; ++i) 
    {
        HashTable* hash_table = HashCtor (4096, funcs[i].func);
        if (!hash_table)
        {
            free (words);
            return 1;
        }
        char out_filename[256] = {};
        snprintf (out_filename, sizeof (out_filename), "%s.txt", funcs[i].name);
        CompletelyHashFuncs (hash_table, words, &word_count, out_filename);
        size_t found = SearchWordsHashTable (hash_table, test_words, test_word_count, 1);
        printf ("[%s] Found: %zu out of %d\n\n", 
                funcs[i].name, found, test_word_count);
        HashDtor (hash_table);
    }

    free (words);

    return 0;
}