#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

typedef struct block
{
    size_t size;        // size of the block, including the header.
    struct block *next; // pointer to the next block in the list.
    int free;           // 1 if free, 0 if allocated.
} block_t;

void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);

#endif // MALLOC_H