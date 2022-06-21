#include <primesieve.h>

#include "list.h"
#include "table.h"

/*
 * this function creates and returns a new hashtable
 *
 * the first parameter is a size hint, which is a rough approximation of what size the user wants. a prime that is
 * greater than the hint is used as the initial hash table size. choosing a prime number as the length of a hash table
 * greatly reduces the chances of key collision, especially if the prime number is large.
 *
 * the second, third, and fourth parameters are all function pointers that enable polymorphic behavior. this structure
 * should be adaptable. i.e. users should be able to have the table search, print, and compare by their own metrics
 *
 */
struct hashtable *hashtable(u64 size_hint, hashfunc hash, cmpfunc comparator, printfunc printer) {
    i32                 status;
    // heap allocate memory for the hashtable
    struct hashtable    *table = malloc(sizeof(struct hashtable));
    // create a new primesieve iterator, initialize it, and skip to the size hint
    primesieve_iterator it;
    primesieve_init(&it);
    primesieve_skipto(&it, size_hint, size_hint * 2);
    // get the next prime greater than the size hint
    u64 prime = primesieve_next_prime(&it);
    // set the initial table variables: initial size is zero. initial capacity is the prime number generated above.
    // initial empty chains are just a chunk of memory, and the function pointers for hashing, printing, and
    // comparing are set
    table->size     = 0;
    table->capacity = prime;
    table->chains   = malloc(table->capacity * sizeof(struct list));
    table->hash     = hash;
    table->print    = printer;
    table->compare  = comparator;
    // initialize the table's latch (mutex) and abort if this was unsuccessful
    status = pthread_mutex_init(&table->latch, NULL);
    if (status != 0) err_abort(status, "unable to initialize latch for table");
    // initialize the lists
    for (int i = 0; i < table->capacity; i++) {
        struct list *chain = list(comparator, printer);
        table->chains[i] = chain;
    }
    // return the table
    return table;
}

/*
 * this function adds a new entry to the hashtable passed in
 *
 * the first parameter is a (hopefully) non-null pointer to a hashtable and the second parameter is just a void pointer
 * that can be anything the user wants
 */
void hashtable_add(struct hashtable *table, const void *entry) {
    i32 status;
    // just exit if something invalid was passed in
    if (!table || !entry) {
        printf("either the table or entry was null\n");
        return;
    }
    // get the index by hashing the entry with the user-provided hash function
    u64 index = table->hash(entry, table->capacity);
    // lock the table's latch or block until it locks
    status = pthread_mutex_lock(&table->latch);
    if (status != 0) err_abort(status, "error locking mutex");
    list_add(table->chains[index], entry);
    // add
    if (table->chains[index]->size < 2) table->size++;
    // release latch on table
    pthread_mutex_unlock(&table->latch);
}

/*
 * this function removes an entry from the hashtable passed in
 *
 * the first parameter is a (hopefully) non-null pointer to a hashtable and the second parameter is a const void
 * pointer to what the user wants to remove.
 *
 */
const void *hashtable_remove(struct hashtable *table, const void *target) {
    u64 index = table->hash(target, table->capacity);
    pthread_mutex_lock(&table->latch);
    struct node *n = list_remove(table->chains[index], target);
    if (table->chains[index]->size == 0) table->size--;
    pthread_mutex_unlock(&table->latch);
    return n ? n->data : NULL;
}

/*
 * this function is similar to the remove function and contains the same parameters. the difference is just that the
 * element is not removed from the table (just found and returned). the user is discouraged from modifying this pointer
 * in any way because, for example, setting it to null or freeing it would probably break the table in strange ways
 */
const void *hashtable_search(struct hashtable *table, const void *target) {
    u64 index = table->hash(target, table->capacity);
    pthread_mutex_lock(&table->latch);
    struct node *n = list_search(table->chains[index], target);
    if (table->chains[index]->size == 0) table->size--;
    pthread_mutex_unlock(&table->latch);
    return n ? n->data : NULL;
}

/*
 * this function resizes the table to the parameter passed in. it is highly encouraged that what is passed in is a
 * prime number larger than the table's current size (if it is full/near full) or smaller (if the table's entries are
 * much fewer than its capacity).
 */
struct hashtable *hashtable_resize(struct hashtable *table, u64 skip_to) {
    struct hashtable *new_table = hashtable(skip_to, table->hash, table->compare, table->print);
    for (int         i          = 0; i < table->capacity; i++) {
        if (table->chains[i]->size > 0) {
            u64 hash = table->hash(table->chains[i]->head->data, new_table->capacity);
            new_table->chains[hash] = table->chains[i];
            new_table->size++;
        }
    }
    return new_table;
}

/*
 * this function iterates over the table and prints each chain in order
 */
void hashtable_print(struct hashtable *table) {
    for (int i = 0; i < table->capacity; i++) {
        if (table->chains[i]->size > 0) {
            printf("%d: ", i);
            list_print(table->chains[i]);
        }
    }
}