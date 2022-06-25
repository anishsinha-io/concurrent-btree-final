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

static const char *build_table_path(const char *table_name) {
    char *path = malloc(strlen(TABLE_PATH) + strlen(table_name) + strlen(".bin"));
    strncpy(path, TABLE_PATH, strlen(TABLE_PATH));
    strncat(path, table_name, strlen(table_name));
    strncat(path, ".bin", strlen(".bin"));
    return path;
}

static const char *build_index_path(const char *table_name) {
    char *path = malloc(strlen(INDEX_PATH) + strlen(table_name) + strlen("_index.bin"));
    strncpy(path, INDEX_PATH, strlen(INDEX_PATH));
    strncat(path, table_name, strlen(table_name));
    strncat(path, "_index.bin", strlen("_index.bin"));
    return path;
}

i32 format_table(const char *table_name, const void *header, size_t header_size) {
    if (!table_name || !header || header_size < 0) return EINVAL;
    const char *table_path = build_table_path(table_name);
    i32        fd          = open(table_path, O_RDWR);
    if (fd < 0 && errno == ENOENT) fd = open(table_path, O_CREAT | O_EXCL | O_WRONLY);
    else truncate(table_path, 0);
    pwrite(fd, header, header_size, 0);
    close(fd);
    return 0;
}

i32 format_index(const char *table_name, const void *header, size_t header_size) {
    if (!table_name || !header || header_size < 0) return EINVAL;
    const char *index_path = build_index_path(table_name);
    i32        fd          = open(index_path, O_RDWR);
    if (fd < 0 && errno == ENOENT) fd = open(index_path, O_CREAT | O_EXCL | O_WRONLY);
    else truncate(index_path, 0);
    pwrite(fd, header, header_size, 0);
    close(fd);
    return 0;
}

