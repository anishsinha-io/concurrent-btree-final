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
** This file is part of the `ioutil` module. This file contains declarations, type definitions, and useful macros for
** interacting with the file system. The implementation of these declarations can be found in src/ioutil.c.
**
*/

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