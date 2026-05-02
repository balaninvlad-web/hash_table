#include "hash_table.h"
#include "read_file.h"
#include "list_on_signs_func.h"

int main (int argc, char* argv[])
{
    const char* filename = argv[1];
    size_t buffer_size = 0;
    printf ("1\n");
    char* buffer = ReadFile (filename, &buffer_size);
    printf ("2\n");
    if (!buffer) return 1;

    int word_count = 0;
    printf ("3\n");
    char** words = ReadWordsFromBuffer (buffer, &buffer_size, &word_count);
    printf ("4\n");
    if (!words)
    {
        free (buffer);
        return 1;
    }
    printf ("5\n");
    printf ("Count words: %d\n", word_count);
    printf ("first's 20:\n");
    for (int i = 0; i < word_count && i < 20; ++i) 
    {
        printf ("%s\n", words[i]);
    }

    HashTable* hash_table = HashCtor (4096, /*какая то хеш футкция*/);

    for (int i = 0; i < word_count; ++i) HashInsert (hash_table, words[i]);

    HashPrintStats (hash_table);
    
    free (words);
    free (buffer);

    return 0;
}