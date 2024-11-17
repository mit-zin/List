#include <stdio.h>

#include "errors.h"

void PrintErr(unsigned int res)
{
    printf("Errors:\n");
    if (res & NULL_PTR)
        printf("Null pointer.\n");
    if (res & UNKNOWN_ERROR)
        printf("Unknown error.\n");
    printf("\n");
}
