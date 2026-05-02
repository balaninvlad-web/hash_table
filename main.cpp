#include "hash_table.h"
#include "read_file.h"
#include "list_on_signs_func.h"
#include "hash_funcs.h"

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        printf ("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    size_t buffer_size = 0;
    char* buffer = ReadFile (filename, &buffer_size);
    if (!buffer) return 1;

    int word_count = 0;
    char** words = ReadWordsFromBuffer (buffer, &buffer_size, &word_count);
    if (!words)
    {
        free (buffer);
        return 1;
    }
    printf ("Count words: %d\n", word_count);
    printf ("first's 20:\n");
    for (int i = 0; i < word_count && i < 20; ++i) 
    {
        printf ("%s\n", words[i]);
    }

    HashTable* hash_table = HashCtor (4096, HashGnu);
    if (!hash_table)
    {
        free (words);
        free (buffer);
        return 1;
    }

    for (int i = 0; i < word_count; ++i) 
        HashInsert (hash_table, words[i]);

    HashPrintStats (hash_table);

    HashDtor (hash_table);
    free (words);
    free (buffer);

    return 0;
}