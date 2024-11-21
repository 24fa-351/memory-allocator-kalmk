#include <assert.h>
#include <stdio.h>

#include "malloc.h"
#include "test.h"

void test_my_malloc()
{
    printf("Running my_malloc test...\n");
    int *ptr = (int *)my_malloc(sizeof(int) * 10);
    assert(ptr != NULL); // ensure the allocation was successful.
    ptr[0] = 100;
    printf("my_malloc test passed: ptr[0] = %d\n", ptr[0]);
    my_free(ptr);
}

void test_my_free()
{
    printf("Running my_free test...\n");
    int *ptr = (int *)my_malloc(sizeof(int) * 5);
    my_free(ptr);
    printf("my_free test passed\n");
}

void test_my_realloc()
{
    printf("Running my_realloc test...\n");
    int *ptr = (int *)my_malloc(sizeof(int) * 5);
    ptr = (int *)my_realloc(ptr, sizeof(int) * 10);
    assert(ptr != NULL);
    printf("my_realloc test passed\n");
    my_free(ptr);
}

void run_tests()
{
    test_my_malloc();
    test_my_free();
    test_my_realloc();
}
