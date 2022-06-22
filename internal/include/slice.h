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

typedef i8(*slice_cmpfunc)(const void *, const void *);

struct slice {
    u64           length, capacity;
    void          **keys;
    slice_cmpfunc compare;
    printfunc     print;
};

struct slice_key_index {
    void *key;
    u64  index;
};


#endif