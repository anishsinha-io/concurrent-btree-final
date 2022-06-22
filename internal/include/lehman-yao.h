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
    bool leaf;
    struct node_entry entries[ORDER];
};


#endif