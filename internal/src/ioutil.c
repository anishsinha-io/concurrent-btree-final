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
** This file is part of the `ioutil` module. This part of the module contains implementations of functions used for
** interacting with the file system, resolving paths, creating and formatting disk files, and reading data from the
** disk meaningfully into memory buffers.
*/

#include "ioutil.h"

struct buffer_pool *buffer_pool(allocator page_allocator) {
    struct buffer_pool *pool = malloc(sizeof(struct buffer_pool));
    pool->alloc_page = page_allocator;
    return pool;
}

i32 write_buffer(const char *path, const void *buf, size_t buf_size, off_t offset) {
    if (!path || !buf || buf_size < 0) return EINVAL;
    i32 fd = open(path, O_WRONLY);
    if (fd < 0) return errno;
    pwrite(fd, buf, buf_size, offset);
    close(fd);
    return 0;
}

