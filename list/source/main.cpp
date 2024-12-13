#include <stdio.h>

#include "list.h"
#include "list_funcs.h"
#include "../../common/include/errors.h"
#include "../../common/include/utilities.h"

int main()
{
    #ifdef DEBUG
        unsigned int res = 0;
    #endif

    List_t list = {};

    WRITE_ER(CreateList(&list));

    WRITE_ER(Insert(&list, 42, 0));
    WRITE_ER(Insert(&list, 13, 1));
    WRITE_ER(Insert(&list, 69, 1));
    WRITE_ER(Insert(&list, 12345, 0));
    WRITE_ER(Delete(&list, 2));
    WRITE_ER(Insert(&list, 333, 3));
    WRITE_ER(Delete(&list, 0));

    WRITE_ER(DUMP_LIST(&list));
    WRITE_ER(DestroyList(&list));

    #ifdef DEBUG
        if (res) PrintErr(res);
    #endif

    printf("End of program.\n");

    return 0;
}
