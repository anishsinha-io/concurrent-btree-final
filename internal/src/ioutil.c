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

/*
 * this function takes in a specified table name parameter as a string and returns the fully resolved path to the
 * database table file corresponding to that name, whether it exists or not
 */
static const char *build_table_path(const char *table_name) {
    char *path = malloc(strlen(TABLE_PATH) + strlen(table_name) + strlen(".bin"));
    strncpy(path, TABLE_PATH, strlen(TABLE_PATH));
    strncat(path, table_name, strlen(table_name));
    strncat(path, ".bin", strlen(".bin"));
    return path;
}

/*
 * this function takes in a specified table name parameter as a string and returns the fully resolved path to the
 * database index file corresponding to that name, whether it exists or not
 */
static const char *build_index_path(const char *table_name) {
    char *path = malloc(strlen(INDEX_PATH) + strlen(table_name) + strlen("_index.bin"));
    strncpy(path, INDEX_PATH, strlen(INDEX_PATH));
    strncat(path, table_name, strlen(table_name));
    strncat(path, "_index.bin", strlen("_index.bin"));
    return path;
}

/*
 * this function takes a given table name and record size and formats a disk file with a table header
 */
i32 format_table(const char *table_name, size_t rec_size) {
    if (!table_name || rec_size < 0) return EINVAL;
    const char *table_path = build_table_path(table_name);
    i32        fd          = open(table_path, O_RDWR);
    if (fd < 0 && errno == ENOENT) fd = open(table_path, O_CREAT | O_EXCL | O_WRONLY);
    else truncate(table_path, 0);
    struct table_header new_table_header;
    new_table_header.rec_size       = rec_size;
    new_table_header.rec_ct         = 0;
    new_table_header.current_max_id = 0;
    strncpy((char *) new_table_header.table_name, table_name, sizeof(((struct table_header *) 0)->table_name));
    pwrite(fd, &new_table_header, sizeof(struct table_header), 0);
    close(fd);
    return 0;
}

/*
 * this function takes a given table name and pointer to a table header buffer and reads in the table header of the
 * database file with the passed in table name. it returns 0 on success, EINVAL if invalid pointers are passed in, or
 * EBADF (bad file descriptor) if the file is unable to be opened or found
 */
i32 read_table_header(const char *table_name, struct table_header *header) {
    if (!table_name || !header) return EINVAL;
    const char *path = build_table_path(table_name);
    i32        fd;
    if ((fd = open(path, O_RDONLY)) < 0) return EBADF;
    pread(fd, header, sizeof(struct table_header), 0);
    close(fd);
    return 0;
}

/*
 * this function creates a new record out of the buffer (rec) provided and appends it to the end of the database table
 * file passed in. returns EINVAL or EBADF on error, and 0 on success
 */
i32 write_record(const char *table_name, const void *rec) {
    i32 status;
    if (!table_name || !rec) return EINVAL;
    struct table_header header;
    status = read_table_header(table_name, &header);
    if (status != 0) return status;
    const char *path = build_table_path(table_name);
    i32        fd    = open(path, O_RDWR);
    if (fd < 0) return EBADF;
    off_t offset = (off_t) (header.rec_ct * header.rec_size + sizeof(header));
    pwrite(fd, rec, header.rec_size, offset);
    close(fd);
    return 0;
}

/*
 * this function reads a database table record into the buffer provided, given that the arguments are valid
 */
i32 read_record(const char *table_name, u64 loc, void *buf) {
    i32 status;
    if (!table_name || !buf || loc < 0) return EINVAL;
    const char          *path = build_table_path(table_name);
    struct table_header header;
    status = read_table_header(table_name, &header);
    if (status != 0) return status;
    if (loc > header.rec_ct) return EINVAL;
    i32 fd = open(path, O_RDONLY);
    if (fd < 0) return EBADF;
    off_t offset = ((off_t) header.rec_size * (off_t) loc + (off_t) sizeof(header));
    pread(fd, (void *) buf, header.rec_size, offset);
    close(fd);
    return 0;
}

/*
 * this function formats the index corresponding to the given table name passed in. a table may or may not have an
 * index, but an index must have a corresponding table, so if that table does not exist, the function will return an
 * error status code.
 */
i32 format_index(const char *table_name) {
    i32 status;
    if (!table_name) return EINVAL;
    struct table_header header;
    status = read_table_header(table_name, &header);
    if (status != 0) return status;
    const char *index_path = build_index_path(table_name);
    i32        fd          = open(index_path, O_WRONLY);
    if (fd < 0 && errno == ENOENT) fd = open(index_path, O_CREAT | O_EXCL | O_WRONLY);
    struct index_header header_index;
    strncpy((char *) &header_index.table_name, table_name, sizeof(((struct index_header *) 0)->table_name));
    header_index.root_loc = -1;
    header_index.height   = -1;
    header_index.node_ct  = 0;
    header_index.order = ORDER;
    pwrite(fd, &header_index, sizeof(header_index), 0);
    return 0;
}