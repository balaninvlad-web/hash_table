#include "read_file.h"

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
char** ReadWordsFromBuffer (char* buffer, size_t* buffer_size, int* word_count)
{
    assert (buffer);
    assert (buffer_size);

    char* ptr = buffer;
    int inside_word = 0;
    int count = 0;

    for (size_t i = 0; i < *buffer_size; ++i)
    {
        if (isalpha ((unsigned char) ptr[i]))
        {
            if (!inside_word)
            {
                inside_word = 1;
                ++count;
            }
        }
        else
        {
            if (inside_word) inside_word = 0;
            ptr[i] = '\0';
        }
    
    }

    char** words = (char**) calloc (count + 1, sizeof (char*));

    if (!words)
    {
        printf ("ReadWordsFromBuffer calloc ERROR");
        free (buffer);
        buffer = NULL;
        return NULL;
    }

    inside_word = 0;
    int index = 0;
    for (size_t i = 0; i < *buffer_size; i++)
    {
        if (isalpha ((unsigned char) ptr[i]))
        {
            if (!inside_word)
            {
                inside_word = 1;
                words[index++] =&ptr[i];
            }
        }
        else
        {
                inside_word = 0;
        }
        
    }

    words[index] = NULL;
    *word_count = count;
    return words;
}