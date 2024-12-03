#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "malloc.h"

size_t align(size_t size)
{
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

void *get_me_blocks(ssize_t how_much)
{
    if (free_list_index >= MAX_BLOCKS)
    {
        return NULL;
    }
    void *ptr = &memory_pool[free_list_index];
    free_list_index++;
    return ptr;
}

void *malloc(size_t size)
{
    size = align(size);

    for (int i = 0; i < free_list_index; ++i)
    {
        BlockHeader *block = (BlockHeader *)&memory_pool[free_list[i]];
        if (block->free && block->size >= size)
        {
            block->free = 0;
            return (void *)((uintptr_t)block + HEADER_SIZE);
        }
    }

    BlockHeader *new_block = (BlockHeader *)get_me_blocks(size + HEADER_SIZE);
    if (!new_block)
    {
        return NULL;
    }

    new_block->size = size;
    new_block->free = 0;
    free_list[free_list_index - 1] =
        (uintptr_t)new_block - (uintptr_t)memory_pool;
    return (void *)((uintptr_t)new_block + HEADER_SIZE);
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;

    BlockHeader *block = (BlockHeader *)((uintptr_t)ptr - HEADER_SIZE);
    block->free = 1;

    free_list[free_list_index] = (uintptr_t)block - (uintptr_t)memory_pool;
    free_list_index++;

    for (int i = 0; i < free_list_index - 1; ++i)
    {
        BlockHeader *current = (BlockHeader *)&memory_pool[free_list[i]];
        BlockHeader *next = (BlockHeader *)&memory_pool[free_list[i + 1]];

        if ((uintptr_t)current + current->size + HEADER_SIZE == (uintptr_t)next)
        {
            current->size += next->size + HEADER_SIZE;
            free_list[i + 1] = free_list[i];
        }
    }
}

// Realloc implementation
void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
    {
        return malloc(size);
    }

    if (size == 0)
    {
        free(ptr);
        return NULL;
    }

    BlockHeader *block = (BlockHeader *)((uintptr_t)ptr - HEADER_SIZE);
    size = align(size);

    if (block->size >= size)
    {
        return ptr;
    }

    void *new_ptr = malloc(size);
    if (new_ptr)
    {
        for (size_t i = 0; i < block->size; i++)
        {
            ((char *)new_ptr)[i] = ((char *)ptr)[i];
        }

        free(ptr);
    }

    return new_ptr;
}
