#include "transaction.h"

void transaction_print(struct transaction *transaction) {
    printf("{");
    printf("\n\ttransaction_id: %llu", transaction->transaction_id);
    printf("\n\tlock_mode: %s", transaction->mode == shared ? "shared" : "exclusive");
    printf("\n\trequest status: %s", transaction->stat == pending ? "pending" : "granted");
    printf("\n}\n");
}

struct transaction *transaction(u64 page_no, enum lock_mode mode) {
    struct transaction *transaction = malloc(sizeof(struct transaction));
    transaction->transaction_id = time(NULL);
    transaction->page_no        = page_no;
    transaction->mode           = mode;
    transaction->stat           = pending;
    return transaction;
}