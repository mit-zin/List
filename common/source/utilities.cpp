#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"

void *Recalloc(void *data, size_t prev_size, size_t new_size, size_t elem_size)
{
    MY_ASSERT(data, "Null pointer given as argument.", exit(EXIT_FAILURE));

    data = realloc(data, new_size * elem_size);
    MY_ASSERT(data, "Reallocation failed.", exit(EXIT_FAILURE));

    for (size_t i = prev_size * elem_size; i < new_size * elem_size; i++)
        *(char *) (data + i) = 0;

    return data;
}
