#ifndef __IOUTIL
#define __IOUTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "errors.h"

#define TABLE_PATH "/Users/anishsinha/Home/scratch/internal-final/test_database/"
#define INDEX_PATH "/Users/anishsinha/Home/scratch/internal-final/test_database/"


struct table_header {
    const char table_name[50];
    u64        rec_ct, current_max_id;
    size_t     rec_size;
};

#define TABLE_NAME_SIZE sizeof(((struct table_header*)0)->table_name)

struct index_header {
    const char index_name[56];
    const char table_name[50];
    i64        root_loc, height;
    u64        node_ct;
    u64        order;
};

i32 format_table(const char *table_name, size_t rec_size);
i32 read_table_header(const char *table_name, struct table_header *header);

#endif