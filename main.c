#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "malloc.h"

int main()
{
    /////////////////////////////////////////// test my_malloc and my_free

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

    //////////////////////// Testing the simple realloc implementation
    int *arr = (int *)sbrk(sizeof(int) * 5);
    for (int i = 0; i < 5; ++i)
    {
        arr[i] = i;
    }

    // Reallocate to a larger size
    arr = (int *)my_realloc(arr, sizeof(int) * 10);

    // Fill the newly allocated memory
    for (int i = 5; i < 10; ++i)
    {
        arr[i] = i;
    }

    // Print the array
    for (int i = 0; i < 10; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Free the memory (using sbrk, just reduce the size)
    sbrk(-sizeof(int) * 10);

    return 0;
}
