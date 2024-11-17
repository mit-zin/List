#include <assert.h>
#include <stdbool.h>

#include "verify_funcs.h"


errors_t Dump(Stack_t *stack, ON_DEBUG(unsigned int stk_errors,) const char *file_name, int line_num,
              const char *func_name)
{
    FILE *dump_out = 0;
    static bool first_file_opening = true;
    if (first_file_opening)
    {
        dump_out = fopen("./dump.txt", "w"); //TODO: MAGIC STRING
        first_file_opening = false;
    }
    else
        dump_out = fopen("./dump.txt", "a");

    if (!dump_out)
        return FILE_NULL_PTR;

#ifdef DEBUG
    if (stk_errors & STACK_NULL_PTR)
        fprintf(dump_out, "Error: stack null pointer.\n");
    if (stk_errors & DATA_NULL_PTR)
        fprintf(dump_out, "Error: data null pointer.\n");
    if (stk_errors & SIZE_BIGGER_CAPACITY)
        fprintf(dump_out, "Error: size is bigger than capacity.\n");
    if (stk_errors & STACK_CANARY_DAMAGED)
        fprintf(dump_out, "Error: stack canary damaged.\n");
    if (stk_errors & DATA_CANARY_DAMAGED)
        fprintf(dump_out, "Error: data canary damaged.\n");
#endif

    fprintf(dump_out, "stack [%#p]\n", stack);
    if (!stack)
        return SUCCESS;

    ON_DEBUG(fprintf(dump_out, "CANARY: %X\n", stack->left_canary);)
    ON_DEBUG(fprintf(dump_out, "CANARY: %X\n", stack->right_canary);)
    fprintf(dump_out, "\tcalled from %s:%d (%s)\n", file_name, line_num, func_name);
    fprintf(dump_out, "\tcapacity = %zu\n", stack->capacity);
    fprintf(dump_out, "\tsize = %zu\n", stack->size);
    fprintf(dump_out, "\tdata [%#p]\n", stack->data);
    if (stack->data)
    {
        ON_DEBUG(fprintf(dump_out, "\t\tCANARY: %X\n", (int)stack->data[-1]);)
        for (size_t i = 0; i < stack->capacity; i++)
            fprintf(dump_out, "\t\t[%zu] = %f\n", i, stack->data[i]);
        ON_DEBUG(fprintf(dump_out, "\t\tCANARY: %X\n", (int)stack->data[stack->capacity]);)
    }

    fprintf(dump_out, "\n\n\n");

    fclose(dump_out);

    return SUCCESS;
}

#ifdef DEBUG
unsigned int VerifyStack(Stack_t *stack)
{
    unsigned int stk_errors = 0;

    if (!stack)
        stk_errors |= STACK_NULL_PTR;

    if (!stack->data)
        stk_errors |= DATA_NULL_PTR;

    if (stack->capacity < stack->size)
        stk_errors |= SIZE_BIGGER_CAPACITY;

    if (stack->data[-1] != CANARY || stack->data[stack->capacity] != CANARY)
        stk_errors |= DATA_CANARY_DAMAGED;

    if (stack->left_canary != CANARY || stack->right_canary != CANARY)
        stk_errors |= STACK_CANARY_DAMAGED;

    return stk_errors;
}
#endif
