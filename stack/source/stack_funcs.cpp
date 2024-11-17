#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack_funcs.h"

errors_t CreateStack(Stack_t *stack, size_t capacity)
{
    if (!stack)
        return NULL_PTR;

    if (capacity < MIN_CAPACITY)
        return WRONG_ARG;

    ON_DEBUG(stack->left_canary = CANARY;)
    ON_DEBUG(stack->right_canary = CANARY;)

    stack->data = (stack_elem_t *) calloc(capacity ON_DEBUG(+ 2), sizeof(stack_elem_t));
    if (!stack->data)
        return NULL_PTR;
    ON_DEBUG(stack->data += 1;)

    FillPoison(stack->data, capacity, POISON);

    stack->size = 0;
    stack->capacity = capacity;
    stack->initial_cpct = capacity;

    ON_DEBUG(stack->data[-1] = (stack_elem_t)CANARY;)
    ON_DEBUG(stack->data[stack->capacity] = (stack_elem_t) CANARY;)

    ON_DEBUG(VERIFY_STACK(stack);)

    return SUCCESS;
}

errors_t StackPush(Stack_t *stack, stack_elem_t pushed_elem)
{
    ON_DEBUG(VERIFY_STACK(stack);)

    errors_t res;
    if (stack->size == stack->capacity && !(res = StackRecalloc(stack)))
        return res;

    stack->data[stack->size] = pushed_elem;
    stack->size++;

    ON_DEBUG(VERIFY_STACK(stack);)

    return SUCCESS;
}

errors_t StackPop(Stack_t *stack, stack_elem_t *poped_elem)
{
    if (!poped_elem)
        return NULL_PTR;

    ON_DEBUG(VERIFY_STACK(stack);)

    *poped_elem = stack->data[stack->size - 1];
    stack->data[stack->size - 1] = POISON;
    stack->size--;


    errors_t res;
    if (stack->size < stack->capacity / STACK_RECALLOC_COEF &&
        stack->capacity / STACK_RECALLOC_COEF > stack->initial_cpct && !(res = StackRecalloc(stack)))
        return res;

    ON_DEBUG(VERIFY_STACK(stack);)

    return SUCCESS;
}

static errors_t StackRecalloc(Stack_t *stack)
{
    ON_DEBUG(VERIFY_STACK(stack);)
    //DUMP(stack, 0);

    if (stack->capacity > MAX_CAPACITY / STACK_RECALLOC_COEF)
        return IS_FULL;

    ON_DEBUG(stack->data--;)

    if (stack->size == stack->capacity)
    {
        stack->data = (stack_elem_t *) Recalloc(stack->data,
            stack->capacity ON_DEBUG(+ 2),
            stack->capacity * STACK_RECALLOC_COEF ON_DEBUG(+ 2), sizeof(stack_elem_t));

        FillPoison(stack->data + stack->capacity ON_DEBUG(+ 1),
                   stack->capacity * (STACK_RECALLOC_COEF - 1), POISON);

        stack->capacity *= STACK_RECALLOC_COEF;
    }
    else if (stack->size < stack->capacity / STACK_RECALLOC_COEF)
    {
        stack->data = (stack_elem_t *) Recalloc(stack->data,
            stack->capacity ON_DEBUG(+ 2),
            stack->capacity / STACK_RECALLOC_COEF ON_DEBUG(+ 2), sizeof(stack_elem_t));

        stack->capacity /= STACK_RECALLOC_COEF;
    }

    if (!stack->data)
            return NULL_PTR;

    ON_DEBUG(stack->data++;)

    ON_DEBUG(stack->data[stack->capacity] = CANARY;)

    ON_DEBUG(VERIFY_STACK(stack);)

    return SUCCESS;
}

errors_t FillPoison(stack_elem_t *data, size_t number, stack_elem_t poison)
{
    if (!data)
        return NULL_PTR;

    for (size_t i = 0; i < number; i++)
        data[i] = poison;

    return SUCCESS;
}

errors_t DestroyStack(Stack_t *stack)
{
    ON_DEBUG(VERIFY_STACK(stack);)

    free(stack->data ON_DEBUG(- 1));
    stack->data = 0;

    stack->size = 0;
    stack->capacity = 0;

    return SUCCESS;
}
