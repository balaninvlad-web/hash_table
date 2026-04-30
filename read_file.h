#ifndef WORD_LOADER_H
#define WORD_LOADER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

char* ReadFile (const char* filename, size_t* out_size);
char** ReadWordsFromBuffer (char* buffer, size_t* buffer_size, int* word_count);

#endif