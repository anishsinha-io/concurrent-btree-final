#include "index.h"

i32 format_index(const char *path, struct b_link_header *header) {
    if (!path || !header) return EINVAL;
    truncate(path, 0);
    write_buffer(path, header, sizeof(*header), 0);
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