#ifndef HASH_H
#define HASH_H

#include <stdint.h>

typedef struct {
    const char *key;    /* heap allocated string */
    const char *val;    /* heap allocated string */
    int8_t status;     /* 0 (empty), 1 (full), 2 (deleted) */
} HashEntry;

typedef struct {
    HashEntry *table;   /* array of HashEntries */
    int length;         /* num entries at any given time */
    int size;           /* max size */
} HashMap;

HashMap *hash_create(int size);

/* Allocates new memory to store val */
void hash_insert(const char *key, const char *val);

/* Fills buffer w/ retrieved item, Buffer can be NULL */
int hash_retrieve(const char *key, char *buffer);   

/* Same as retrieve, but deletes item */
int hash_delete(const char *key, char *buffer);

/* Destroyes table, frees all associated memory */
void hash_destroy();

/* Returns the status of index */
int hash_status(int index);

/* Returns current table size */
int hash_size();


#endif // DEBUG
