#ifndef VERIFY_FUNCS_H
#define VERIFY_FUNCS_H

#include "../../common/include/errors.h"
#include "stack.h"

const int CANARY = (stack_elem_t) 0XF00D;

typedef enum
{
    STACK_NULL_PTR       = 1,
    DATA_NULL_PTR        = 2,
    SIZE_BIGGER_CAPACITY = 4,
    STACK_CANARY_DAMAGED = 8,
    DATA_CANARY_DAMAGED  = 16
} stack_errors_t;

unsigned int VerifyStack(Stack_t *stack);
errors_t     Dump(Stack_t *stack, ON_DEBUG(unsigned int stk_errors,) const char *file_name,
                  int line_num, const char *func_name);

#ifdef DEBUG
    #define DUMP(STACK, STK_ERRORS) Dump(STACK, STK_ERRORS, __FILE__, __LINE__, __func__)
#else
    #define DUMP(STACK, StK_ERRORS) Dump(STACK, __FILE__, __LINE__, __func__)
#endif

#define VERIFY_STACK(STACK) do                       \
    {                                                \
        unsigned int stk_errors = VerifyStack(STACK);\
        if (DUMP(STACK, stk_errors))                 \
            printf("Error: dump file null pointer.");\
        if (stk_errors)                              \
            assert(0);                               \
    } while(0)

#endif

