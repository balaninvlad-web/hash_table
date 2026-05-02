#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H

#include <stdint.h>

uint64_t HashAlwaysOne (const char* key);
uint64_t HashFirstChar (const char* key);
uint64_t HashLength (const char* key);
uint64_t HashSum (const char* key);
uint64_t HashRotateLeft (const char* key);
uint64_t HashRotateRight (const char* key);
uint64_t HashGnu (const char* key);      // DJB2
uint64_t HashCrc32 (const char* key);    // аппаратный CRC32 (SSE4.2)

#endif