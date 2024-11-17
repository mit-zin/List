#ifndef UTILITIES_H
#define UTILITIES_H

#include "errors.h"

const int RECALLOC_COEF = 2;

void *Recalloc(void *data, size_t prev_size, size_t new_size, size_t elem_size);

#endif
