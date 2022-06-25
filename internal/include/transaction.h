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
** This file is part of the `transaction` module. This file contains declarations, type definitions, and useful macros
** for working with transaction objects. The implementation of these declarations can be found in src/transaction.c.
**
*/

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

u64 transaction_hash(const struct transaction *t1, u64 size);
bool transaction_compare(const struct transaction *t1, const struct transaction *t2);
void transaction_print(struct transaction *transaction);
struct transaction *transaction(u64 page_no, enum lock_mode mode);

#endif