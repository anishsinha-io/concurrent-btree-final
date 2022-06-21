#ifndef __LIST
#define __LIST

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "errors.h"


typedef void(*printfunc)(const void *);
typedef bool(*cmpfunc)(const void *, const void *);

struct node {
    const void  *data;
    struct node *prev, *next;
};

struct list {
    u64         size;
    struct node *head, *tail;
    cmpfunc     compare;
    printfunc   print;
};

enum l_stat {
    L_EINVAL = 1, L_ENOMEM = 2, L_EINS = 3, L_EDEL = 4,
};

struct list *list(cmpfunc compare, printfunc print);
struct node *list_remove(struct list *list, const void *target);
struct node *list_search(struct list *list, const void *target);
void list_print(struct list *list);
void list_add(struct list *list, const void *entry);

#endif