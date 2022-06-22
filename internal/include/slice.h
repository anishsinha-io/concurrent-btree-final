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