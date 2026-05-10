#include "read_file.h"

char** FullWordLoading (const char* filename, int* word_count, char** out_buffer)
{
    size_t buffer_size = 0;
    char* buffer = ReadFile (filename, &buffer_size);
    if (!buffer) return NULL;

    char** words = ReadWordsFromBuffer (&buffer, &buffer_size, word_count);
    if (!words)
    {
        free (buffer);
        return NULL;
    }
    #ifndef NDEBUG
        printf ("Count words: %d\n", *word_count);
        printf ("first's 20:\n");
        for (int i = 0; i < *word_count && i < 20; ++i) 
        {
            printf ("%s\n", words[i]);
        }
    #endif
    
    *out_buffer = buffer;

    return words;
}

char* ReadFile (const char* filename, size_t* out_size)
{
    FILE* file = fopen (filename, "rb");

    if (!file) 
    {
        printf ("Cannot open file...\n");
        return 0;
    }

    fseek (file, 0, SEEK_END);

    long size = ftell (file);

    if (size < 0) 
    {
        fclose(file);
        printf ("ReadFile ftell ERROR");
        return NULL;
    }
    
    rewind (file);

    char* buffer = (char*) calloc (size + 1, 1);
    if (!buffer)
    {
        fclose (file);
        printf ("ReadFile calloc ERROR");
        return NULL;
    }
    size_t bytes = fread (buffer, 1, size, file);
    fclose (file);

    buffer[size] = '\0';
    *out_size = size;

    return buffer; 
}

char** ReadWordsFromBuffer(char** buffer, size_t* buffer_size, int* word_count)
{
    assert (buffer);
    assert (*buffer);
    assert (buffer_size);
    assert (word_count);
   
    int inside = 0;
    int cnt = 0;
    for (size_t i = 0; i < *buffer_size; ++i) 
    {
        if (isalpha ((unsigned char) (*buffer) [i])) 
        {
            if (!inside) 
            { 
                inside = 1; 
                ++cnt; 
            }
        } 
        else 
        {
            if (inside) inside = 0;
            (*buffer)[i] = '\0';
        }
    }
   
    char* aligned = (char*) _aligned_malloc ((size_t)cnt * 32, 32);
    if (!aligned) return NULL;
    
    char** words = (char**) calloc (cnt, sizeof(char*));
    if (!words) 
    {
        _aligned_free (aligned);
        return NULL;
    }
 
    inside = 0;
    int idx = 0;
    for (size_t i = 0; i < *buffer_size; ++i) 
    {
        if (isalpha ((unsigned char) (*buffer)[i])) 
        {
            if (!inside) 
            {
                inside = 1;
                size_t len = 0;
               
                while ((*buffer)[i + len] != '\0' && len < 31) ++len;
                memcpy (aligned + idx * 32, &(*buffer)[i], len);
                aligned[idx * 32 + len] = '\0';
                words[idx] = aligned + idx * 32;
                ++idx;
            }
        } else inside = 0;
    }
    
    free (*buffer);
    *buffer = aligned;
    *word_count = cnt;
    return words;
}