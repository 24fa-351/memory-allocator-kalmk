#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "malloc.h"

// the free list to track available blocks
static Block *free_list = NULL;

// simple malloc implementation using sbrk
void *my_malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    size_t total_size = size + sizeof(Block);

    // search the free list for a suitable block
    Block *prev = NULL;
    Block *curr = free_list;

    while (curr != NULL)
    {
        if (curr->size >= total_size)
        {
            if (prev != NULL)
            {
                prev->next = curr->next;
            }
            else
            {
                free_list = curr->next;
            }
            return (void *)((char *)curr +
                            sizeof(Block)); // return a pointer to the memory
                                            // after the header
        }
        prev = curr;
        curr = curr->next;
    }

    // no suitable block found, request more memory from the system using sbrk
    Block *new_block = (Block *)sbrk(total_size);

    if (new_block == (void *)-1)
    {
        return NULL;
    }

    new_block->size = total_size;
    new_block->next = NULL;

    return (void *)((char *)new_block + sizeof(Block));
}

// simple free implementation
void my_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    Block *block = (Block *)((char *)ptr - sizeof(Block));

    block->next = free_list;
    free_list = block;
}