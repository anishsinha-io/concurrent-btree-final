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
    new_table_header.num_records    = 0;
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