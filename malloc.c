#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void *my_realloc(void *ptr, size_t new_size)
{
    if (new_size == 0)
    {
        if (ptr)
        {
            free(ptr);
        }
        return NULL;
    }

    // if ptr is NULL, realloc behaves like malloc
    if (!ptr)
    {
        return sbrk(new_size);
    }

    // if new_size is the same as the old size, no need to do anything
    // note: this is a very simplistic assumption, doesn't handle fragmentation
    size_t old_size =
        *(size_t *)ptr -
        sizeof(size_t); // save old size (assuming it's stored at the start)

    if (new_size == old_size)
    {
        return ptr;
    }

    // allocate a new block using sbrk
    void *new_ptr = sbrk(new_size);
    if (new_ptr == (void *)-1)
    {
        return NULL; // sbrk failure
    }

    // copy old data to the new location (if increasing size)
    if (new_size < old_size)
    {
        memcpy(new_ptr, ptr, new_size);
    }
    else
    {
        memcpy(new_ptr, ptr, old_size);
    }

    // free the old memory
    sbrk(-old_size);

    return new_ptr;
}