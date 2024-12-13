#ifndef STACK_T_H
#define STACK_T_H

#include <stdio.h>
#include <limits.h>

#include "../../common/include/errors.h"

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

typedef double stack_elem_t;

const stack_elem_t POISON = (stack_elem_t) 0XB0B15DEAD;

const size_t MIN_CAPACITY = 1;
const size_t MAX_CAPACITY = UINT_MAX;

typedef struct
{
    ON_DEBUG(int left_canary;)
    stack_elem_t* data;
    size_t size, capacity, initial_cpct;
    ON_DEBUG(int right_canary;)
} Stack_t;

const int STACK_RECALLOC_COEF = 2;

#endif
