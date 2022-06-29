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
** This file is part of the `lehman-yao` module. This file contains declarations, type definitions, and useful macros
** for working with b-link tree indexes. The implementation of these declarations can be found in src/lehman-yao.c.
**
*/

#ifndef __LEHMAN_YAO
#define __LEHMAN_YAO

#include <stdio.h>
#include <stdlib.h>

#include "slice.h"
#include "stack.h"
#include "errors.h"
#include "global.h"
#include "defaults.h"

struct b_link_header {
    char   table_name[50];
    u64    node_ct;
    i64    height;
    size_t node_size;
};

struct b_link_node {
    bool         leaf;
    u64          order;
    struct slice *keys, *children;
};

struct node_entry {
    u64 id, loc;
};

struct b_link_header *b_link_header(char *table_name, size_t node_size);
struct b_link_node *b_link_node(bool leaf, u64 min_order);
const char *b_link_header_to_string(struct b_link_header *header);

#endif