#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

// define a simple memory block structure
typedef struct Block
{
    size_t size;        // size of the block, including the header
    struct Block *next; // pointer to the next block in the free list
} Block;

void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t new_size);

#endif // MALLOC_H