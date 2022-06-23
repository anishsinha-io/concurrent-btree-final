### This file contains a description and proof for the locking protocol used in this project

The objective of this proof is to demonstrate that no matter which database operations or how many operations take place
concurrently on the database table file and database index file, the integrity of the database table and index files
will always be intact. That is, a thread will not ever be able to see a state where some change has been reflected in
the index but not in the database and vice versa. All file states will be consistent.

#### Terminology

- A database table file refers to a file used to store data in a binary, tabular format on the disk. A table consists of
  a table header, which contains metadata about the table, and rows. The important metadata includes the following: the
  size of each record, the number of records, the highest current primary key, and the table name. Each row has a
  primary key and some data.
- A database index file refers to a file used to index a table. The file consists of an index header and zero or more
  index nodes, which form a tree. The exact structure of a tree is a B-Link tree, originally called a Lehman and Yao
  Tree, which is a tree well-suited to many concurrent operations at once. Each index node contains a boolean variable
  indicating whether it is a leaf, and an array of node entries. Each node entry contains two `unsigned long long`
  integer values, the first of which is the field to be indexed (the primary key), and the second of which functions as
  a pointer. On internal nodes, this pointer points to the location of the next index node to go to, and on leaf nodes
  this points to the location on the database file where the item corresponding to the primary key is located.
- The database operations mentioned in the objective are defined by the following set S = {search, modify},
  where `modify` is a generalization of insertion and deletion.
- A lock table refers to an in-memory data structure, represented as a hash table, which is used to keep track of all
  locks on the database. More information on the lock table is contained in the separate lock table description file.
- A mutual exclusion lock (mutex) is a synchronization primitive that is used to enforce mutual exclusion among threads
  working on a common object. Before any thread works on an object, it must first lock the object's mutex. A mutex is
  used on the lock table, for example, so that no threads can either view or modify the lock table if it hasn't locked
  the mutex on the lock table first.
- A read-write lock (rwlock) refers to a synchronization primitive refers to a synchronization primitive that has two
  modes: shared read mode and exclusive write mode. A rwlock may have any number of shared readers, but only one
  exclusive writer. When a write lock is acquired on the rwlock, all new readers and writers will block until the write
  operation finishes. When there are any readers, any write will block until the readers are all released. Read
  precedence favors concurrency because it potentially allows many threads to accomplish work simultaneously [1]. The
  implementation of rwlock that is used in this project is from the pthreads (POSIX threads) API. The implementation
  used supports the following operations: initialization, destruction, acquire_readlock, try_acquire_readlock,
  release_readlock, acquire_writelock, try_acquire_writelock, and release_writelock. The pthreads API uses slightly
  different names but the spirit remains the same.

#### Searching the Database

To search the database, the calling thread performs a standard B-Link tree search as described in Lehman and Yao's
paper [2]. No locks are necessary to simply search the database, so that operation doesn't require any difficult action
outside just performing the search. The result of the search will either be a pointer to a location on the database or
-1, which indicates "not found". Once the search has returned, on the chance that a valid pointer was found, the thread
will need to acquire a shared read lock on the database. To do this, it must first go through the lock table.

##### Lemma 1.01: No thread can acquire a lock without first registering it with the lock table

When a thread performs a search of the database, it must acquire a mutex on the lock table first. The thread will block
until the mutex is acquired. Until the mutex on the lock table is acquired, it will be unable to register a lock with
the lock table because that is the defined behavior of pthreads mutexes as described in details in the POSIX standard.
Upon successfully acquiring the lock, the thread will wake up and now has exclusive access to the lock table.

Once the thread has exclusive access to the lock table, it needs to hash the name of the database file it wants "read"
access to and obtain the index this corresponds to in the hash table. Once it does this, a transaction object is
appended to the chain of that index.  

