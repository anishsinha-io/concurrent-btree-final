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
#include "table.h"

#define TABLE_PATH "/Users/anishsinha/Home/scratch/internal-final/test_database/"
#define INDEX_PATH "/Users/anishsinha/Home/scratch/internal-final/test_database/"

#define FRAME_COUNT 64

typedef void *(*allocator)(size_t size);

struct frame {
    u64  frame_no;
    char buf[1000];
};

struct page_meta {
    u64    frame_no, page_no;
    size_t frame_offset, page_offset;
    u8     modified: 1;
};

struct buffer_pool {
    u64              max_pages;
    u64              free_pages;
    cmpfunc          compare_page;
    printfunc        print_page;
    allocator        alloc_page;
    struct hashtable *pool_map;
    struct frame     frames[FRAME_COUNT];
};

struct buffer_pool *buffer_pool(allocator page_allocator);

#endif