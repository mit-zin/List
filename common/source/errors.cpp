#include <stdio.h>

#include "errors.h"

void PrintErr(unsigned int res)
{
    printf("Errors:\n");
    if (res & NULL_PTR)
        printf("Null pointer.\n");
    if (res & UNKNOWN_ERROR)
        printf("Unknown error.\n");
    if (res & FILE_NULL_PTR)
        printf("File opening error.\n");
    if (res & TOO_BIG_IND)
        printf("Given index is bigger than the size of list.\n");
    if (res & IS_FULL)
        printf("No space for new elements.\n");
    if (res & WRONG_ARG)
        printf("Wrong argument.\n");

    printf("\n");
}
