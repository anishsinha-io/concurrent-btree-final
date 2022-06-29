#include "lehman-yao.h"
#include "stringbuilder.h"

struct b_link_header *b_link_header(char *table_name, size_t node_size) {
    struct b_link_header *header = malloc(sizeof(struct b_link_header));
    strncpy(header->table_name, table_name, sizeof(header->table_name));
    header->node_size = node_size;
    header->node_ct   = 0;
    header->height    = -1;
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
    sb->appendstring(sb, "\n\theight: ");
    sb->appendlong(sb, header->height);
    sb->appendstring(sb, "\n}");
    return (const char *) sb->buf;
}

static int compare_u64(const void *first, const void *second) {
    return (int) (*(u64 *) first - *(u64 *) second);
}

static void print_u64(const void *el) {
    printf("%llu\n", *(u64 *) el);
}

struct b_link_node *b_link_node(bool leaf, u64 min_order) {
    struct b_link_node *node = malloc(sizeof(struct b_link_node));
    node->keys     = slice(compare_u64, print_u64);
    node->children = slice(compare_u64, print_u64);
    node->order    = min_order;
    node->leaf     = leaf;
    return node;
}

static i32 insert_key_safe(struct b_link_node *node, struct node_entry *entry) {
    
}

static i32 insert_key_unsafe(struct b_link_node *node, struct node_entry *entry) {

}
