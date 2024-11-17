#include <stdio.h>

#include "list.h"
#include "list_funcs.h"
#include "../../common/include/errors.h"
#include "../../common/include/utilities.h"

int main()
{
    List_t list = {};
    CreateList(&list);
    Insert(&list, 42, 0);
    Insert(&list, 13, 1);
    Insert(&list, 69, 1);
    Insert(&list, 12345, 0);

    Dump(&list);
    DestroyList(&list);

    printf("End of program.\n");

    return 0;
}
