#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

typedef struct block
{
    size_t size;        
    struct block *next; 
    int free;           // 1 if free, 0 if allocated.
} block_t;

void *get_me_blocks(ssize_t how_much);
void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);

#endif // MALLOC_H