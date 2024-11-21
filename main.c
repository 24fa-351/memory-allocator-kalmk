#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "malloc.h"
#include "test.h"

int main(int argc, char *argv[])
{
    if (argc == 3 && strcmp(argv[1], "-t") == 0)
    {
        if (strcmp(argv[2], "my_malloc") == 0)
            test_my_malloc();
        else if (strcmp(argv[2], "my_free") == 0)
            test_my_free();
        else if (strcmp(argv[2], "my_realloc") == 0)
            test_my_realloc();
    }
    else
    {
        run_tests();
    }

    return 0;
}