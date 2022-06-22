#ifndef __TRANSACTION
#define __TRANSACTION

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errors.h"

enum lock_mode {
    shared = 1, exclusive = 2
};

enum req_stat {
    pending = 1, granted = 2
};

enum operation {
    search = 1, insert = 2, delete = 3
};

struct transaction {
    u64            transaction_id, page_no;
    enum lock_mode mode;
    enum req_stat  stat;
    enum operation operation;
};

void transaction_print(struct transaction *transaction);
struct transaction *transaction(u64 page_no, enum lock_mode mode);

#endif