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

#include "errors.h"

/*
 * the following is a struct describing an entry inside an index node
 *
 * a node entry contains two items, both of type u64: a primary key and a loc. on leaf nodes, the loc points to a
 * place in a database table file. on internal nodes, the loc points to the location of another node in the index. they
 * both function exactly like pointers, but rather than a memory address, they are plain u64s.
 */
struct node_entry {
    u64 primary_key, loc;
};

/*
 * the following is a struct describing an index node
 *
 * an index node contains an array of node entries as well as a boolean value which states whether the node is a leaf
 * or not.
 */
struct index_node {
    bool              leaf;
    struct node_entry entries[ORDER];
};

struct index_node *index_node();


#endif