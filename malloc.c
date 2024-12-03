#include <stddef.h> 
#include <stdint.h>
#include <unistd.h> 

#include "malloc.h"

static BlockHeader *free_list = NULL;

size_t align(size_t size)
{
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

void *malloc(size_t size)
{
    size = align(size);

    BlockHeader *current = free_list;
    BlockHeader *prev = NULL;

    while (current)
    {
        if (current->free && current->size >= size)
        {
            if (current->size > size + HEADER_SIZE)
            {
                BlockHeader *new_block =
                    (BlockHeader *)((uintptr_t)current + HEADER_SIZE + size);
                new_block->size = current->size - size - HEADER_SIZE;
                new_block->free = 1;
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }

            current->free = 0;
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                free_list = current->next;
            }

            return (void *)((uintptr_t)current + HEADER_SIZE);
        }

        prev = current;
        current = current->next;
    }

    size_t total_size = size + HEADER_SIZE;
    current = (BlockHeader *)sbrk(total_size);
    if (current == (void *)-1)
    {
        return NULL;
    }

    current->size = size;
    current->free = 0;
    current->next = NULL;

    return (void *)((uintptr_t)current + HEADER_SIZE);
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;

    BlockHeader *block = (BlockHeader *)((uintptr_t)ptr - HEADER_SIZE);
    block->free = 1;

    block->next = free_list;
    free_list = block;

    BlockHeader *current = free_list;
    while (current && current->next)
    {
        if ((uintptr_t)current + current->size + HEADER_SIZE ==
            (uintptr_t)current->next)
        {
            current->size += current->next->size + HEADER_SIZE;
            current->next = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }
}

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
