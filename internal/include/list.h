/*
** June 17, 2022
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
***********************************************************************************************************************
**
** This file is part of the `list` module. This file contains declarations, type definitions, and useful macros for
** working with linked list objects. The implementation of these declarations can be found in src/list.c.
**
*/

#ifndef __LIST
#define __LIST

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "errors.h"


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