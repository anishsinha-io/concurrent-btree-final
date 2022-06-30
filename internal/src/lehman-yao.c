#include "lehman-yao.h"
#include "stringbuilder.h"

struct b_link_header *b_link_header(char *table_name, size_t node_size) {
    struct b_link_header *header = malloc(sizeof(struct b_link_header));
    strncpy(header->table_name, table_name, sizeof(header->table_name));
    header->node_size = node_size;
    header->node_ct   = 0;
    header->height    = header->root_loc = 0;
    return header;
}


const char *b_link_header_to_string(struct b_link_header *header) {
    struct stringbuilder *sb = stringbuilder();
    sb->appendstring(sb, "{");
    sb->appendstring(sb, "\n\ttable_name: ");
    sb->appendstring(sb, header->table_name);
    sb->appendstring(sb, "\n\tnode_ct: ");
    sb->appendlong(sb, (long long) header->node_ct);
    sb->appendstring(sb, "\n\tnode_size: ");
    sb->appendlong(sb, (long long) header->node_size);
    sb->appendstring(sb, "\n\troot_loc: ");
    sb->appendlong(sb, (long long) header->root_loc);
    sb->appendstring(sb, "\n\theight: ");
    sb->appendlong(sb, header->height);
    sb->appendstring(sb, "\n}");
    return (const char *) sb->buf;
}

const char *b_link_node_to_string(struct b_link_node *node) {
    struct stringbuilder *sb = stringbuilder();
    sb->appendstring(sb, "{");
    sb->appendstring(sb, "\n\tleaf: ");
    sb->appendstring(sb, node->leaf ? "true" : "false");
    sb->appendstring(sb, "\n\tid: ");
    sb->appendlong(sb, (long long) node->id);
    sb->appendstring(sb, "\n\torder: ");
    sb->appendlong(sb, (long long) node->order);
    sb->appendstring(sb, "\n\tnum_keys: ");
    sb->appendlong(sb, (long long) node->num_keys);
    sb->appendstring(sb, "\n\thigh_key: ");
    sb->appendlong(sb, (long long) node->high_key);
    sb->appendstring(sb, "\n\tloc: ");
    sb->appendlong(sb, node->loc);
    sb->appendstring(sb, "\n\tnext_loc: ");
    sb->appendlong(sb, node->next_loc);
    sb->appendstring(sb, "\n\tkeys: [");
    for (int i = 0; i < node->num_keys; i++) {
        if (i == node->num_keys - 1) {
            sb->appendlong(sb, (long long) node->keys[i]);
            break;
        }
        sb->appendlong(sb, (long long) node->keys[i]);
        sb->appendstring(sb, ", ");
    }
    sb->appendstring(sb, "]");
    sb->appendstring(sb, "\n\tchildren: [");
    sb->appendstring(sb, "]");
    if (!node->leaf) {
        for (int i = 0; i < node->num_keys + 1; i++) {
            if (i == node->num_keys) {
                sb->appendlong(sb, (long long) node->children[i]);
                break;
            }
            sb->appendstring(sb, "\t");
            sb->appendlong(sb, (long long) node->children[i]);
            sb->appendstring(sb, ",\n");
        }
    }
    sb->appendstring(sb, "\n}");
    return sb->buf;
}


struct b_link_node *b_link_node(u64 min_order) {
    struct b_link_node *node = malloc(sizeof(struct b_link_node));
    node->order    = min_order;
    node->leaf     = true;
    node->high_key = -1;
    node->next_loc = node->loc = -1;
    return node;
}