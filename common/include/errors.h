#ifndef ERRORS_H
#define ERRORS_H

#define DEBUG
#define ON_ASSERT

typedef enum
{
    SUCCESS       = 0,
    UNKNOWN_ERROR = 1,
    NULL_PTR      = 2,
    FILE_NULL_PTR = 4,
    TOO_BIG_IND   = 8,
    IS_FULL       = 16,
    WRONG_ARG     = 32
} errors_t;

#ifdef ON_ASSERT
#define MY_ASSERT(CHECKED_ARG, MESSAGE, ACTION) do                                         \
    {                                                                                      \
        if (!(CHECKED_ARG))                                                                \
        {                                                                                  \
            printf("Assertion failed: " #CHECKED_ARG "\nMessage: " MESSAGE                 \
                   "\nFile: " __FILE__ "\nFunction: %s\nLine: %d\n\n", __func__, __LINE__);\
            ACTION;                                                                        \
        }                                                                                  \
    } while(0)
#else
    #define MY_ASSERT(CHECKED_ARG, MESSAGE, ACTION)
#endif

#ifdef DEBUG
#define CHECK_ER(FUNC) do   \
    {                       \
        errors_t res = FUNC;\
        if (res)            \
            return res;     \
    } while(0)
#else
    #define CHECK_ER(FUNC) FUNC
#endif

#ifdef DEBUG
#define WRITE_ER(FUNC) do      \
    {                          \
        res |= FUNC;           \
    }while(0)
#else
    #define WRITE_ER(FUNC) FUNC
#endif


void PrintErr(unsigned int res);

#endif
