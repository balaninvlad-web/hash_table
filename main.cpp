#include "hash_table.h"
#include "read_file.h"
#include "list_on_signs_func.h"
#include "hash_funcs.h"

#define HASH_FUNC_ENTRY(func) { #func, func }

#ifdef BENCHMARK_MODE
    #define SEARCH_REPETITIONS 1000000
    #define SAVE_DISTRIBUTIONS 0
    HashFuncInfo funcs[] = { HASH_FUNC_ENTRY (HashCrc32) };
    const size_t func_count = 1;  
#else
    #define SEARCH_REPETITIONS 200
    #define SAVE_DISTRIBUTIONS 1
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
    const size_t func_count = sizeof (funcs) / sizeof (funcs[0]);
#endif

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf ("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    int word_count = 0;
    char* main_buffer = NULL;
    char** words = FullWordLoading (argv[1], &word_count, &main_buffer);
    if (!words) return 1;
    
    int test_word_count = 0;
    char* test_buffer = NULL;
    char** test_words = FullWordLoading (argv[2], &test_word_count, &test_buffer);
    if (!test_words) return 1;

    for (size_t i = 0; i < func_count; ++i) 
    {
        HashTable* hash_table = HashCtor (4096, funcs[i].func);
        if (!hash_table)
        {
            free (words);
            free (test_words);
            free (main_buffer);
            free (test_buffer);
            return 1;
        }
        char out_filename[256] = {};
        snprintf (out_filename, sizeof (out_filename), "%s.txt", funcs[i].name);
        CompletelyHashFuncs (hash_table, words, &word_count, out_filename);
        unsigned long long start = __rdtsc();
        size_t found = SearchWordsHashTable (hash_table, test_words, test_word_count, SEARCH_REPETITIONS);
        unsigned long long end = __rdtsc();
        unsigned long long total_ticks = end - start;
        printf ("[%s] Found %zu / %zu, ticks: %llu, per search: %.2f\n",
                 funcs[i].name, found, (size_t)test_word_count * 200,
                 total_ticks, (double)total_ticks / (test_word_count * 200));
        HashDtor (hash_table);
    }

    free (words);
    free (test_words);
    free (main_buffer);
    free (test_buffer);

    return 0;
}