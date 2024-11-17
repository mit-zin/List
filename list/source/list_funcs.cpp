#include <stdio.h>
#include <stdlib.h>

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

    return SUCCESS;
}

errors_t Insert(List_t *list, int elem, size_t index)
{
    MY_ASSERT(list, "Null pointer given as argument.", return NULL_PTR);

    if (list->size == list->capacity)
        return IS_FULL;

    if (list->size < index)
        return TOO_BIG_IND;

    stack_elem_t free = 0;
    CHECK_ER(StackPop(&list->free, &free));

    list->cell[(int)free].data = elem; // Fill data

    list->cell[(int)free].next = list->cell[index].next;
    list->cell[(int)free].prev = index;
    list->cell[list->cell[index].next].prev = (int)free;

    list->cell[index].next = (int)free;

    list->tail = list->cell[0].prev;


    /*if (list->size == 1)
    {
        list->cell[list->free].data = elem;
        list->cell[list->free].next = 0;
        list->free++;
    }
    else if (list->cell[index].next == 0)
    {
        list->cell[list->free].data = elem;
        list->tail = list->free;
        list->cell[index].next = list->free;
        list->tail = list->free;
        list->free = list->cell[list->free].next;
        list->cell[list->cell[index].next].next = 0;
        list->cell[list->cell[index].next].prev = index;

    }
    else
    {
        list->cell[list->free].data = elem;
        int next_free = list->cell[list->free].next;

        //list->tail = list->free;

        list->cell[list->free].next = list->cell[index].next;
        list->cell[list->free].prev = index;

        list->cell[list->cell[index].next].prev = list->free;
        list->cell[index].next = list->free;

        list->free = next_free;

    }*/

    list->size++;

    return SUCCESS;
}

errors_t Dump(List_t *list)
{
    MY_ASSERT(list, "Null pointer given as argument.", return NULL_PTR);

    FILE *dot_file = fopen(DOT_FILE_NAME, "w");
    if (dot_file == 0)
        return FILE_NULL_PTR;

    CHECK_ER(Draw(list, dot_file));

    fclose(dot_file);

    system("dot dump.dot -Tsvg -o dump.svg");

    FILE *html_file = fopen(HTML_FILE_NAME, "w");
    if (html_file == 0)
        return FILE_NULL_PTR;

    fprintf(html_file, "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Dump</title>\n\t</head>\n"
            "\t<body>\n\t\t<img\n\t\t\tsrc=\"dump.svg\"\n\t\t\twidth=\"1700\"\n"
            "\t\t\theight=\"550\"/>\n\t\t<pre>\n");

    fprintf(html_file, "\thi\n");

    fprintf(html_file, "\t\t</pre>\n\t</body>\n</htnl>\n");

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


    stack_elem_t free = 0;
    CHECK_ER(StackPop(&list->free, &free));
    fprintf(dot_file, "\n\t\tfree[shape=tripleoctagon, style=filled, width=0.5, height=0.4, "
                      "fontsize=15, penwidth=0.7, fillcolor=\"#5069ab\", group=g2];\n");
    fprintf(dot_file, "\t\tfree:s -> %zu:n[arrowhead=curve, style=dotted, penwidth=1.3, "
                      "arrowsize=1];\n\n", (int)free);
    CHECK_ER(StackPush(&list->free, free));

    fprintf(dot_file, "\t\thead[shape=doubleoctagon, style=filled, width=0.6, height=0.4, "
                      "fontsize=15, fontcolor=\"#0a1355\", group=g2, "
                      "penwidth=0.7, fillcolor=\"#b7913c\"];\n");
    fprintf(dot_file, "\t\thead:s -> %zu:n[arrowhead=onormal, style=dotted, penwidth=1.3, "
                      "arrowsize=1];\n\n", list->head);

    fprintf(dot_file, "\t\ttail[shape=doubleoctagon, style=filled, width=0.5, height=0.4, group=g2, "
                      "fontsize=15, fontcolor=\"#0a1355\", penwidth=0.7, fillcolor=\"#b7913c\"];\n");
    fprintf(dot_file, "\t\ttail:s -> %zu:n[arrowhead=crow, style=dotted, penwidth=1.3, "
                      "arrowsize=1];\n\n", list->tail);

    fprintf(dot_file, "\t\rinf[label=\"size: %zu | capacity: %zu\", style=dashed, fontsize=14];\n"
                      "{rank=same; inf; %zu;}\n", list->size, list->capacity, list->capacity / 2);


    fprintf(dot_file, "\t}\n");
    fprintf(dot_file, "}\n");

    return SUCCESS;
}

errors_t DestroyList(List_t *list)
{
    MY_ASSERT(list, "Null pointer given as argument", return NULL_PTR);

    free(list->cell);
    CHECK_ER(DestroyStack(&list->free));

    return SUCCESS;
}
