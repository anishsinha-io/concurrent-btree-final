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
** This file is part of the `slice` module. This file contains declarations, type definitions, and useful macros for
** working with slice objects. The implementation of these declarations can be found in src/slice.c.
**
*/

#ifndef __SLICE
#define __SLICE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "errors.h"

#define INITIAL_SLICE_CAPACITY 10

typedef i32(*slice_cmpfunc)(const void *, const void *);

struct slice {
    u64           length, capacity;
    void          **keys;
    slice_cmpfunc compare;
    printfunc     print;
};

u64 slice_find_index(const struct slice *s, const void *key);
u64 slice_len(const struct slice *s);
void slice_insert_front(struct slice *s, void *key);
void slice_insert_back(struct slice *s, void *key);
void *slice_delete_front(struct slice *s);
void *slice_delete_back(struct slice *s);
void slice_insert_index(struct slice *s, void *key, u64 index);
void *slice_delete_index(struct slice *s, u64 index);
void slice_set_index(struct slice *s, void *key, u64 index);
void *slice_get_index(struct slice *s, u64 index);
void slice_join(struct slice *s1, struct slice *s2);
void slice_print(struct slice *s);
void slice_print(struct slice *s);
void slice_sort(struct slice *s);
void slice_to_array(struct slice *s, void **array, u64 array_length);
void slice_to_primitive_array(struct slice *s, void *array, u64 array_length, size_t key_size);
struct slice *slice(slice_cmpfunc compare, printfunc print);
struct slice *subslice(struct slice *s, u64 start, u64 end);
struct slice *slice_from_array(slice_cmpfunc compare, printfunc print, void **keys, u64 num_keys);
struct slice *
slice_from_primitive_array(slice_cmpfunc compare, printfunc print, void *keys, u64 num_keys, size_t key_size);

#endif