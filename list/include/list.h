#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#include "../../stack/include/stack.h"

const int LIST_POISON = 0XB00B5;

typedef struct
{
    int data, next, prev;
} Cell_t;

typedef struct
{
    Cell_t *cell;
    int head, tail;
    Stack_t free;
    size_t size, capacity;
    FILE *html_file;
} List_t;

const int INITIAL_SIZE = 13;

#endif
