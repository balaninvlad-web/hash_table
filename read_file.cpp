#include "read_file.h"

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

    free (words);
    free (buffer);

    return 0;
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