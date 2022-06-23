#include "errors.h"
#include "lehman-yao.h"

struct index_node *index_node(bool leaf) {
    struct index_node *new_node = malloc(sizeof(struct index_node));
    new_node->leaf = leaf;
    return new_node;
}

struct node_entry *node_entry(u64 primary_key, u64 loc) {
    struct node_entry *new_entry = malloc(sizeof(struct node_entry));
    new_entry->primary_key = primary_key;
    new_entry->loc = loc;
    return new_entry;
}