#include "index.h"

i32 format_index(const char *path, char *table_name, size_t node_size) {
    if (!path || !table_name || node_size < 1) return EINVAL;
    truncate(path, 0);
    struct b_link_header *header = b_link_header(table_name, node_size);
    struct b_link_node   *root   = b_link_node(true, TEST_ORDER);
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
    i32   fd     = open(path, O_WRONLY);
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
    if (loc - 1 > header.node_ct) return EINVAL;
    off_t offset = (off_t) (header.node_size * loc);
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

static i32 read_root(const char *path, struct b_link_node *buf) {
    i32                  status;
    struct b_link_header header;
    if (!path || !buf) return EINVAL;
    status = read_index_header(path, &header);
    if (status != 0) err_abort(status, "unable to read index header into memory");
    // the offset here means the size of the index header + the location of the root multiplied by the size of each
    // index page (each of which is one logical node)
    off_t offset = (off_t) (sizeof(header) + header.root_loc * sizeof(struct b_link_node));
    i32   fd     = open(path, O_RDONLY);
    if (fd < 0) return EBADF;
    pread(fd, buf, sizeof(struct b_link_node), offset);
    return 0;
}

i32 b_link_insert(char *path, struct b_link_entry *entry) {
    i32                  status;
    struct stack         *s = stack(print_u64);
    struct b_link_header header;
    status = read_index_header(path, &header);
    if (status != 0) err_abort(status, "unable to read index header");
    struct b_link_node current;
    status = read_root(path, &current);
    if (status != 0) err_abort(status, "unable to read root node into memory");
    while (!current.leaf) {
        struct b_link_node tmp;
        memcpy(&tmp, &current, sizeof(tmp));
        
    }
}
