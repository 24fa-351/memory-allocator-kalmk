#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "malloc.h"

int main()
{
    // test my_malloc and my_free

    // allocate some memory
    printf("Allocating 100 bytes...\n");
    void *ptr1 = my_malloc(100);
    if (ptr1 != NULL)
    {
        printf("Memory allocated at address: %p\n", ptr1);
    }
    else
    {
        printf("Memory allocation failed!\n");
    }

    // allocate more memory
    printf("Allocating 200 bytes...\n");
    void *ptr2 = my_malloc(200);
    if (ptr2 != NULL)
    {
        printf("Memory allocated at address: %p\n", ptr2);
    }
    else
    {
        printf("Memory allocation failed!\n");
    }

    // free the memory
    printf("Freeing memory at address: %p\n", ptr1);
    my_free(ptr1);

    printf("Freeing memory at address: %p\n", ptr2);
    my_free(ptr2);

    // try to allocate memory again after freeing
    printf("Allocating 50 bytes after free...\n");
    void *ptr3 = my_malloc(50);
    if (ptr3 != NULL)
    {
        printf("Memory allocated at address: %p\n", ptr3);
    }
    else
    {
        printf("Memory allocation failed!\n");
    }

    return 0;
}
