#ifndef __IOUTIL
#define __IOUTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "errors.h"

#define TABLE_PATH "/Users/anishsinha/Home/scratch/lock-table-final/test_database/"
#define INDEX_PATH "/Users/anishsinha/Home/scratch/lock-table-final/test_database/"

#define PAGE_SIZE 1000

struct table_header {
    const char table_name[50];
    u64        num_records, current_max_id;
    size_t     rec_size;
};

#define TABLE_NAME_SIZE sizeof(((struct table_header*)0)->table_name)

struct index_header {
    const char index_name[56];
    const char table_name[50];
};

i32 format_table(const char *table_name, size_t rec_size);
i32 read_table_header(const char *table_name, struct table_header *header);

#endif