#ifndef LIST_FUNCS_H
#define LIST_FUNCS_H

#include "list.h"
#include "../../common/include/utilities.h"
#include "../../common/include/errors.h"
#include "../../stack/include/stack_funcs.h"
#include "../../stack/include/verify_funcs.h"

static const char *DOT_FILE_NAME  = "dump.dot";
static const char *HTML_FILE_NAME = "dump.html";

errors_t CreateList (List_t *list);
errors_t DumpList   (List_t *list, const char *filename, const char* funcname, int line);
errors_t Draw       (List_t *list, FILE *dot_file);
errors_t Insert     (List_t *list, int elem, size_t index);
errors_t Delete     (List_t *list, size_t index);
errors_t DestroyList(List_t *list);

#define DUMP_LIST(LIST) DumpList(LIST, __FILE__, __func__, __LINE__)

#endif
