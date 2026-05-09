#include "hash_funcs.h"
#include <string.h>
#include <assert.h>
#include <nmmintrin.h>   // для _mm_crc32_u8

uint64_t HashAlwaysOne (const char* key) 
{
    assert (key != NULL);
    return 1;
}

uint64_t HashFirstChar (const char* key) 
{
    assert (key != NULL);
    return (uint64_t) (unsigned char) key[0];
}

uint64_t HashLength (const char* key) 
{
    assert (key != NULL);
    uint64_t len = 0;
    while (*key++) len++;
    return len;
}

uint64_t HashSum (const char* key) 
{
    assert (key != NULL);
    uint64_t sum = 0;
    while (*key) sum += (unsigned char)* key++;
    return sum;
}

uint64_t HashRotateLeft (const char* key) 
{
    assert(key != NULL);
    uint8_t ch = (uint8_t)*key;
    uint8_t rotated = (ch << 1) | (ch >> 7);
    return (uint64_t)rotated;
}

uint64_t HashRotateRight (const char* key) 
{
    assert (key != NULL);
    uint8_t ch = (uint8_t) *key;
    uint8_t rotated = (ch >> 1) | (ch << 7);
    return (uint64_t) rotated;
}

uint64_t HashGnu (const char* key) 
{
    assert(key != NULL);
    uint64_t hash = 5381;
    while (*key) hash = (hash << 5) + hash + (unsigned char)*key++;
    return hash;
}

uint64_t HashCrc32 (const char* key) 
{
    assert (key != NULL);

    uint32_t crc = 0xFFFFFFFF;

    while (*key)
    {
        crc ^= (uint8_t) (*key);

        for (int i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
        key++;
    }

    return (uint64_t)~crc;
}