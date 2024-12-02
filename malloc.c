#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "malloc.h"

static block_t *free_list = NULL;

void *get_me_blocks(ssize_t how_much)
{
    void *ptr = sbrk(0);
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
    size_t total_size = size + sizeof(block_t);

    while (curr)
    {
        if (curr->free && curr->size >= total_size)
        {
            if (curr->size > total_size + sizeof(block_t))
            {
                block_t *new_block = (block_t *)((char *)curr + total_size);
                new_block->size = curr->size - total_size;
                new_block->free = 1;
                new_block->next = curr->next;
                curr->size = total_size;
                curr->next = new_block;
            }
            curr->free = 0;
            return (char *)curr + sizeof(block_t);
        }
        prev = curr;
        curr = curr->next;
    }

    block_t *new_block = get_me_blocks(total_size);
    if (!new_block)
        return NULL;

    new_block->size = total_size;
    new_block->free = 0;
    new_block->next = NULL;

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

    block_t *block = (block_t *)((char *)ptr - sizeof(block_t));

    block->free = 1;

    block->next = free_list;
    free_list = block;
}

void *my_realloc(void *ptr, size_t size)
{
    if (!ptr)
        return my_malloc(size);
    if (size == 0)
    {
        my_free(ptr);
        return NULL;
    }

    block_t *block = (block_t *)((char *)ptr - sizeof(block_t));
    size_t total_size = size + sizeof(block_t);

    if (block->size >= total_size)
    {
        if (block->size > total_size + sizeof(block_t))
        {
            block_t *new_block = (block_t *)((char *)block + total_size);
            new_block->size = block->size - total_size;
            new_block->free = 1;
            new_block->next = block->next;
            block->size = total_size;
            block->next = new_block;
        }
        return ptr;
    }

    void *new_ptr = my_malloc(size);
    if (new_ptr)
    {
        memcpy(new_ptr, ptr, block->size - sizeof(block_t));
        my_free(ptr);
    }
    return new_ptr;
}
