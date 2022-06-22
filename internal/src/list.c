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
** This file is part of the `list` module. This file contains implementations of functions used for creating and
** manipulating linked list objects, which are necessary and useful in this project's implementation of the hash table.
** More specifically, the linked list objects here are used to implement separate chaining.
**
*/

#include "list.h"
#include "errors.h"

// create a node given a buffer and entry. return 0 on success
static u8 node(struct node *buf, const void *entry) {
    buf->data = entry;
    buf->next = NULL;
    return 0;
}

// create a list given a buffer, compare function, and print function. return error status code on error and 0 on success
struct list *list(cmpfunc compare, printfunc print) {
    struct list *new_list = malloc(sizeof(struct list));
    new_list->head    = new_list->tail = NULL;
    new_list->compare = compare;
    new_list->print   = print;
    new_list->size    = 0;
    return new_list;
}

// add the passed in entry to the list passed in
void list_add(struct list *list, const void *entry) {
    if (!list || !entry) return;
    struct node *new_node = malloc(sizeof(struct node));
    node(new_node, entry);
    if (list->tail) {
        new_node->prev   = list->tail;
        list->tail->next = new_node;
        list->tail       = new_node;
    } else list->head = list->tail = new_node;
    list->size++;
}

// remove the target from the list passed in and return it if it exists
struct node *list_remove(struct list *list, const void *target) {
    struct node *itr = list->head;
    while (itr && list->compare(itr->data, target) != 0) itr = itr->next;
    if (!itr) return NULL;
    if (list->size == 1) {
        list->head = list->tail = NULL;
        list->size = 0;
        return itr;
    }
    // if we're removing the head
    if (!itr->prev) {
        list->head = list->head->next;
    }
        // if we're removing the tail
    else if (!itr->next) {
        list->tail       = itr->prev;
        list->tail->next = NULL;
    } else {
        itr->prev->next = itr->next;
        itr->next->prev = itr->prev;
    }
    list->size--;
    return itr;
}

struct node *list_search(struct list *list, const void *target) {
    struct node *itr = list->head;
    while (itr && list->compare(itr->data, target) != 0) itr = itr->next;
    return itr;
}

void list_print(struct list *list) {
    struct node *itr = list->head;
    while (itr) {
        list->print(itr->data);
        itr = itr->next;
    }
    printf("\n");
}
