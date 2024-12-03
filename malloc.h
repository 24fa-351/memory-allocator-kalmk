#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>
#include <unistd.h>

#define MAX_BLOCKS 1024
#define ALIGNMENT 8
#define HEADER_SIZE sizeof(BlockHeader)

typedef struct BlockHeader
{
    size_t size;
    int free;
} BlockHeader;

static BlockHeader memory_pool[MAX_BLOCKS];
static int free_list[MAX_BLOCKS];
static int free_list_index = 0;

void *get_me_blocks(ssize_t how_much);
size_t align(size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif // MALLOC_H