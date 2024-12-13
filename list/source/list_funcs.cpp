#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_funcs.h"

errors_t CreateList(List_t *list)
{
    MY_ASSERT(list, "Null pointer given as argument.", return NULL_PTR);

    list->cell = (Cell_t *) calloc(INITIAL_SIZE, sizeof(Cell_t));
    MY_ASSERT(list->cell, "Allocation failed.", return NULL_PTR);
    list->capacity = INITIAL_SIZE;
    list->size = 1;

    list->cell[0].next = 0;
    list->cell[list->capacity - 1].next = 0;
    for (size_t i = 1; i < INITIAL_SIZE - 1; i++)
        list->cell[i].next = i + 1;

    list->cell[0].prev = 0;
    for (size_t i = 1; i < INITIAL_SIZE; i++)
        list->cell[i].prev = i - 1;

    for (size_t i = 0; i < INITIAL_SIZE; i++)
        list->cell[i].data = LIST_POISON;

    list->head = 0;
    list->tail = 0;

    CHECK_ER(CreateStack(&list->free, INITIAL_SIZE));
    for (size_t i = list->capacity - 1; i > 0; i--)
        CHECK_ER(StackPush(&list->free, i));

    list->html_file = fopen(HTML_FILE_NAME, "w");
    if (list->html_file == 0)
        return FILE_NULL_PTR;

    fprintf(list->html_file, "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Dump</title>\n\t</head>\n"
            "\t<body>\n");

    return SUCCESS;
}

errors_t Insert(List_t *list, int elem, size_t index)
{
    MY_ASSERT(list, "Null pointer given as argument.", return NULL_PTR);

    if (list->size == list->capacity)
        return IS_FULL;

    if (list->size <= index)
        return TOO_BIG_IND;

    size_t anchor = 0;
    for (size_t i = 0; i < index; i++)
        anchor = list->cell[anchor].next;

    stack_elem_t free = 0;
    CHECK_ER(StackPop(&list->free, &free));

    list->cell[(int)free].data = elem; // Fill data

    list->cell[(int)free].next = list->cell[anchor].next;
    list->cell[(int)free].prev = anchor;
    list->cell[list->cell[anchor].next].prev = (int)free;

    list->cell[anchor].next = (int)free;

    list->tail = list->cell[0].prev;
    list->head = list->cell[0].next;

    list->size++;

    DUMP_LIST(list);

    return SUCCESS;
}

errors_t Delete(List_t *list, size_t index)
{
    MY_ASSERT(list, "Null pointer given as argument.", return NULL_PTR);

    if (list->size <= index + 1)
        return TOO_BIG_IND;

    size_t anchor = 0;
    for (size_t i = 0; i < index + 1; i++)
        anchor = list->cell[anchor].next;

    list->cell[anchor].data = LIST_POISON;

    list->cell[list->cell[anchor].prev].next = list->cell[anchor].next;
    list->cell[list->cell[anchor].next].prev = list->cell[anchor].prev;

    list->cell[anchor].next = (int) list->free.data[list->free.size - 1];
    list->cell[anchor].prev = 0;

    CHECK_ER(StackPush(&list->free, anchor));

    list->head = list->cell[0].next;
    list->tail = list->cell[0].prev;

    list->size--;

    DUMP_LIST(list);

    return SUCCESS;
}

errors_t DumpList(List_t *list, const char *filename, const char *funcname, int line)
{
    MY_ASSERT(list, "Null pointer given as argument.", return NULL_PTR);

    FILE *dot_file = fopen(DOT_FILE_NAME, "w");
    if (!dot_file)
        return FILE_NULL_PTR;

    CHECK_ER(Draw(list, dot_file));
    fclose(dot_file);

    char cmd[35] = {}; // TODO
    char svg_file[13] = {};
    static int dump_num = 0;
    sprintf(cmd, "dot dump.dot -Tsvg -o dump%d.svg", dump_num);
    sprintf(svg_file, "dump%d.svg", dump_num);
    system(cmd);
    dump_num++;

    fprintf(list->html_file, "\t\t<img\n\t\t\tsrc=\"");
    fprintf(list->html_file, "%s", svg_file);
    fprintf(list->html_file, "\"\n\t\t\twidth=\"1700\"\n\t\t\theight=\"550\"/>\n\t\t<pre>\n"
                             "\tFile:%s\n\tFunction:%s\n\tLine:%d\n\t\t</pre>\n",
                             filename, funcname, line);

    return SUCCESS;
}

errors_t Draw(List_t *list, FILE *dot_file)
{
    MY_ASSERT(list, "Null pointer given as argument.", return NULL_PTR);

    fprintf(dot_file, "digraph\n{\n");

    fprintf(dot_file, "\tpad=0.6;\n\trankdir=\"LR\";\n\tranksep=0.6;\nnodesep=1.2;\n"
                      "\tbgcolor=\"royalblue4\";\n\tcolor=gray99;\n");

    fprintf(dot_file, "\tsubgraph cluster_list\n\t{\n");
    fprintf(dot_file, "\t\tmargin=40;\n\t\tfontsize=37;\n"
                      "\t\tgraph[fontcolor=gray99, bgcolor=\"#1a2365\", penwidth=1.5];\n"
                      "\t\tlabel=\"List\";\n");

    fprintf(dot_file, "\t\tnode[shape=\"Mrecord\", fontsize=15, width=1.6, color=darkgoldenrod2, "
                      "fontcolor=ghostwhite, style=diagonals, penwidth=1.3, fixedsize=true, "
                      "height=1];\n");
    fprintf(dot_file, "\t\tedge[color=darkgoldenrod2, style=dashed, arrowhead=veenoneodiamond, "
                      "arrowsize=0.8, penwidth=1.1];\n\n");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(dot_file, "\t\t%zu[label=\"%zu | data: %d | {<prev> prev:%d | <next> next:%d}\", group=g1]\n",
                i, i, list->cell[i].data, list->cell[i].prev, list->cell[i].next);
    }

    fprintf(dot_file, "\t\t0");
    for (size_t i = 1; i < list->capacity; i++)
        fprintf(dot_file, " -> %zu", i);
    fprintf(dot_file, "[style=\"invis\", weight=10];\n");

    for (size_t i = 0; i < list->capacity - 1; i++)
        fprintf(dot_file, "\t\t%zu:<next>:s -> %zu:<prev>:s[weight=1];\n", i, list->cell[i].next);


    fprintf(dot_file, "\n\t\tfree[shape=tripleoctagon, style=filled, width=0.7, height=0.5, "
                      "fontsize=19, penwidth=0.8, fillcolor=\"#5069ab\", group=g2];\n");
    fprintf(dot_file, "\t\tfree:s -> %zu:n[arrowhead=curve, style=dotted, penwidth=1.3, "
                      "arrowsize=1];\n\n", (int)list->free.data[list->free.size - 1]);

    fprintf(dot_file, "\t\thead[shape=doubleoctagon, style=filled, width=0.8, height=0.5, "
                      "fontsize=19, fontcolor=\"#0a1355\", group=g2, "
                      "penwidth=0.8, fillcolor=\"#b7913c\"];\n");
    fprintf(dot_file, "\t\thead:s -> %zu:n[arrowhead=onormal, style=dotted, penwidth=1.3, "
                      "arrowsize=1];\n\n", list->head);

    fprintf(dot_file, "\t\ttail[shape=doubleoctagon, style=filled, width=0.65, height=0.5, group=g2, "
                      "fontsize=19, fontcolor=\"#0a1355\", penwidth=0.8, fillcolor=\"#b7913c\"];\n");
    fprintf(dot_file, "\t\ttail:s -> %zu:n[arrowhead=crow, style=dotted, penwidth=1.3, "
                      "arrowsize=1];\n\n", list->tail);

    fprintf(dot_file, "\t\tinf[label=\"size: %zu | capacity: %zu\", style=dashed, fontsize=17];\n"
                      "\t\t{rank=same; inf; %zu;}\n", list->size, list->capacity, list->capacity / 2);


    fprintf(dot_file, "\t}\n");
    fprintf(dot_file, "}\n");

    return SUCCESS;
}

errors_t DestroyList(List_t *list)
{
    MY_ASSERT(list, "Null pointer given as argument", return NULL_PTR);

    fprintf(list->html_file, "\t</body>\n</html>\n");
    fclose(list->html_file);

    free(list->cell);
    CHECK_ER(DestroyStack(&list->free));

    return SUCCESS;
}
