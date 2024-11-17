#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H

#include "../../common/include/utilities.h"
#include "../../common/include/errors.h"
#include "stack.h"
#include "verify_funcs.h"

errors_t CreateStack (Stack_t *stack, size_t capacity);
errors_t StackPop    (Stack_t *stack, stack_elem_t *poped_elem);
errors_t StackPush   (Stack_t *stack, stack_elem_t pushed_elem);
errors_t DestroyStack(Stack_t *stack);
static errors_t StackRecalloc(Stack_t *stack);
static errors_t FillPoison(stack_elem_t *data, size_t number, stack_elem_t poison);

#endif
