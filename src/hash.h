#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include "slab.h"

typedef struct {
    const char *key;    /* heap allocated string */
    const char *val;    /* heap allocated string */
    int8_t status;     /* 0 (empty), 1 (full), 2 (deleted) */
} HashEntry;

typedef struct {
    item **table;   /* array of HashEntries */
    int length;         /* num entries at any given time */
    int size;           /* max size */
} HashMap;

HashMap *hash_create(int size);

/**
 * Allocates new memory to store the value
 * @param key The key (string) to get hashed into an index
 * @param val A string to be associated with the provided key
 */
void hash_insert(const char *key, void *item);

/**
 * Fills provied buffer with whatever value is associated with the key. If the
 * buffer is NULL, then the function will just check if the provided key has
 * been inserted
 * @param key The key (string) to get hashed into an index
 * @param val NULLABLE the buffer to be filled with the associated value
 * @return status of retrieval. -1 is an error, 0 is success
 */
void *hash_retrieve(const char *key);   

/**
 * Indexes hashmap with the provided key. If found and the buffer is not NULL,
 * the buffer is filled with the value and the item is removed from the map
 * @param key The key (string) to get hashed into an index
 * @param buffer NULLABLE the buffer to get filled with the associated value
 * @return status of deletion. -1 is an error, 0 is success
 */
void hash_delete(const char *key);

/**
 * Destroys the hashtable, frees all memory managed by the table 
 */
void hash_destroy();

/**
 * Checks the given index for its status:
 *  2: deleted
 *  0: empty
 *  1: filled
 *  @param index Index of the hashtable to be checked
 *  @return status of the hashtable
 */
int hash_status(int index);

/**
 * Returns the current size of the hashtable
 * @return size of the current hashtable
 */
int hash_size();


#endif // DEBUG
