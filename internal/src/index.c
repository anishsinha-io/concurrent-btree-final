#include "index.h"

i32 format_index(const char *path, char *table_name, size_t node_size) {
    if (!path || !table_name || node_size < 1) return EINVAL;
    truncate(path, 0);
    struct b_link_header *header = b_link_header(table_name, node_size);
    struct b_link_node   *root   = b_link_node(TEST_ORDER);
    root->leaf = true;
    write_buffer(path, header, sizeof(*header), 0);
    write_buffer(path, root, sizeof(*root), sizeof(*header));
    return 0;
}

i32 read_index_header(const char *path, struct b_link_header *buf) {
    if (!path || !buf) return EINVAL;
    i32 fd = open(path, O_RDONLY);
    if (fd < 0) return EBADF;
    pread(fd, buf, sizeof(*buf), 0);
    return 0;
}

i32 write_index_page(const char *path, struct b_link_node *buf) {
    if (!path || !buf) return EINVAL;
    i32                  status;
    struct b_link_header header;
    status = read_index_header(path, &header);
    if (status != 0) err_abort(status, "unable to read index header into memory");
    off_t offset = (off_t) (sizeof(header) + header.node_ct * header.node_size);
    buf->loc = (i64) header.node_ct;
    i32 fd = open(path, O_WRONLY);
    if (fd < 0) return EBADF;
    pwrite(fd, buf, sizeof(*buf), offset);
    close(fd);
    return 0;
}

i32 read_index_page(const char *path, u64 loc, struct b_link_node *buf) {
    if (!path || !buf) return EINVAL;
    i32                  status;
    struct b_link_header header;
    status = read_index_header(path, &header);
    if (status != 0) err_abort(status, "unable to read index header into memory");
    if ((i64) loc - 1 > (i64) header.node_ct) return EINVAL;
    off_t offset = (off_t) (sizeof(struct b_link_node) * loc + sizeof(header));
    i32   fd     = open(path, O_RDONLY);
    pread(fd, buf, sizeof(*buf), offset);
    close(fd);
    return 0;
}


static int compare_u64(const void *first, const void *second) {
    return (int) (*(u64 *) first - *(u64 *) second);
}

static void print_u64(const void *el) {
    printf("%llu\n", *(u64 *) el);
}

#define NODE_KEYS(node) slice_from_primitive_array(compare_u64, print_u64, (node)->keys, (node)->num_keys, sizeof(u64))

static u64 scan_node(struct b_link_node *node, struct b_link_entry *entry) {
    if (entry->id > node->high_key) return node->high_key;
    struct slice *node_keys = NODE_KEYS(node);
    u64          index      = slice_find_index(node_keys, &entry->id);
    return *(u64 *) (node_keys->keys[index]);
}

static i32 b_link_insert_recursive(struct b_link_node *page) {

}

i32 b_link_insert(char *path, struct b_link_entry *entry) {
    i32                  status;
    struct stack         *s = stack(print_u64);
    struct b_link_header header;
    status = read_index_header(path, &header);
    if (status != 0) err_abort(status, "unable to read index header");
    u64                current = header.root_loc;
    struct b_link_node A;
    read_index_page(path, current, &A);
    while (!A.leaf) {
        u64 t = current;
        current = scan_node(&A, entry);
        if (current != A.high_key) stack_push(s, &t);
        read_index_page(path, current, &A);
    }
    read_index_page(path, current, &A);
    struct slice *node_keys = NODE_KEYS(&A);
    i64          loc        = slice_search(node_keys, &current, 0, node_keys->length);

    if (loc != -1) {
        printf("key already exists in tree!\n");
        return -1;
    }
    u64 page_loc;

}
