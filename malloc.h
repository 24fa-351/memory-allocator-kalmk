#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

#define ALIGNMENT 8
#define HEADER_SIZE sizeof(BlockHeader)

typedef struct BlockHeader
{
    size_t size;
    struct BlockHeader *next;
    int free;
} BlockHeader;

size_t align(size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif // MALLOC_H