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
** This file is part of the `transaction` module. This file contains implementations for functions used for operating on
** transaction objects, which are the primary objects used to model operations on the database files.
**
*/

#include "transaction.h"

void transaction_print(struct transaction *transaction) {
    printf("{");
    printf("\n\ttransaction_id: %llu", transaction->transaction_id);
    printf("\n\tlock_mode: %s", transaction->mode == shared ? "shared" : "exclusive");
    printf("\n\trequest status: %s", transaction->stat == pending ? "pending" : "granted");
    printf("\n}\n");
}

/*
 * if t1 was created before t2, return true. otherwise, return false
 */
bool transaction_compare(const struct transaction *t1, const struct transaction *t2) {
    return t1->transaction_id < t2->transaction_id;
}

struct transaction *transaction(u64 page_no, enum lock_mode mode) {
    struct transaction *transaction = malloc(sizeof(struct transaction));
    transaction->transaction_id = time(NULL);
    transaction->page_no        = page_no;
    transaction->mode           = mode;
    transaction->stat           = pending;
    return transaction;
}

u64 transaction_hash(const struct transaction *t1, u64 size) {
    u64 aggregate = t1->transaction_id + t1->page_no + t1->operation + rand_r((unsigned int *) &t1->transaction_id);
    return aggregate % size;
}