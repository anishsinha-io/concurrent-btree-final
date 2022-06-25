#ifndef __GLOBAL
#define __GLOBAL

#include <stdbool.h>

#include "transaction.h"
#include "table.h"

struct hashtable *global_table;

#define PREPARE_LOCK_TABLE if (!global_table)\
global_table = hashtable(612, (hashfunc) transaction_hash, (cmpfunc) transaction_compare,\
                         (printfunc) transaction_print);\

#endif