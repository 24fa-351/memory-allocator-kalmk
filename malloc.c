#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "malloc.h"

static block_t *free_list = NULL; // head of the free list.

void *get_me_blocks(ssize_t how_much)
{
    void *ptr = sbrk(0); // get current program break.
    if (sbrk(how_much) == (void *)-1)
    {
        fprintf(stderr, "Failed to get memory\n");
        return NULL;
    }
    return ptr;
}

void *my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    block_t *curr = free_list;
    block_t *prev = NULL;
    size_t total_size = size + sizeof(block_t); // account for metadata size.

    // search for a free block that is large enough.
    while (curr)
    {
        if (curr->free && curr->size >= total_size)
        {
            if (curr->size > total_size + sizeof(block_t))
            {
                // split the block if it is large enough to create a new free
                // block.
                block_t *new_block = (block_t *)((char *)curr + total_size);
                new_block->size = curr->size - total_size;
                new_block->free = 1;
                new_block->next = curr->next;
                curr->size = total_size;
                curr->next = new_block;
            }
            curr->free = 0; // mark the block as allocated.
            return (char *)curr +
                   sizeof(block_t); // return pointer to memory after metadata.
        }
        prev = curr;
        curr = curr->next;
    }

    // no suitable free block found. Request more memory.
    block_t *new_block = get_me_blocks(total_size);
    if (!new_block)
        return NULL;

    new_block->size = total_size;
    new_block->free = 0;
    new_block->next = NULL;

    // add the new block to the free list if necessary.
    if (free_list)
    {
        prev->next = new_block;
    }
    else
    {
        free_list = new_block;
    }

    return (char *)new_block + sizeof(block_t);
}

void my_free(void *ptr)
{
    if (!ptr)
        return;

    block_t *block =
        (block_t *)((char *)ptr - sizeof(block_t)); // get the block metadata.

    block->free = 1; // mark the block as free.

    // add the block to the free list.
    block->next = free_list;
    free_list = block;
}

void *my_realloc(void *ptr, size_t size)
{
    if (!ptr)
        return my_malloc(size); // if the pointer is NULL, behave like malloc.
    if (size == 0)
    {
        my_free(ptr);
        return NULL;
    }

    block_t *block = (block_t *)((char *)ptr - sizeof(block_t));
    size_t total_size = size + sizeof(block_t);

    // if the current block is large enough, just resize it.
    if (block->size >= total_size)
    {
        if (block->size > total_size + sizeof(block_t))
        {
            // split the block if it's large enough to create a new free block.
            block_t *new_block = (block_t *)((char *)block + total_size);
            new_block->size = block->size - total_size;
            new_block->free = 1;
            new_block->next = block->next;
            block->size = total_size;
            block->next = new_block;
        }
        return ptr;
    }

    // if the block is too small, allocate a new one and copy the data.
    void *new_ptr = my_malloc(size);
    if (new_ptr)
    {
        memcpy(new_ptr, ptr,
               block->size - sizeof(block_t)); // copy the old data.
        my_free(ptr);
    }
    return new_ptr;
}
