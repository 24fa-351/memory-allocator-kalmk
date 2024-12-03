#include <assert.h>
#include <stdio.h>

#include "malloc.h"
#include "test.h"

void test_malloc()
{
    printf("Running malloc test...\n");
    int *ptr = (int *)malloc(sizeof(int) * 10);
    assert(ptr != NULL); // ensure the allocation was successful.
    ptr[0] = 100;
    printf("malloc test passed: ptr[0] = %d\n", ptr[0]);
    free(ptr);
}

void test_free()
{
    printf("Running free test...\n");
    int *ptr = (int *)malloc(sizeof(int) * 5);
    free(ptr);
    printf("free test passed\n");
}

void test_realloc()
{
    printf("Running realloc test...\n");
    int *ptr = (int *)malloc(sizeof(int) * 5);
    ptr = (int *)realloc(ptr, sizeof(int) * 10);
    assert(ptr != NULL);
    printf("realloc test passed\n");
    free(ptr);
}

void run_tests()
{
    test_malloc();
    test_free();
    test_realloc();
}
