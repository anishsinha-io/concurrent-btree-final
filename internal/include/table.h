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
** This file is part of the `table` module. This file contains declarations, type definitions, and useful macros for
** working with hash table objects. The implementation of these declarations can be found in src/table.c.
**
*/

#ifndef __TABLE
#define __TABLE

#include <stdbool.h>
#include <pthread.h>

#include "list.h"
#include "errors.h"

// hash function signature

// hash table struct
struct hashtable {
    u64             size, capacity;
    struct list     **chains;
    hashfunc        hash;
    printfunc       print;
    cmpfunc         compare;
    // to ensure thread safety there is a mutex embedded into the hash table
    pthread_mutex_t latch;
};

// insert, delete, and search operations will return status codes from this list or zero if no error occured
enum h_stat {
    H_EINVAL = 1, H_ENOMEM = 2, H_EINS = 3, H_EDEL = 4,
};


const void *hashtable_remove(struct hashtable *h, const void *target);
const void *hashtable_search(struct hashtable *h, const void *target);
void hashtable_print(struct hashtable *table);
void hashtable_add(struct hashtable *h, const void *entry);
struct hashtable *hashtable(u64 size_hint, hashfunc hash, cmpfunc comparator, printfunc printer);

#endif