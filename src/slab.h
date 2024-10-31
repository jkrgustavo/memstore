#ifndef SLAB_H
#define SLAB_H

#define UTIL_ALL
#include "utils.h"

typedef struct _stritem {
    /* implementation of linked list, only contains free items */
    struct _stritem *prev; 
    struct _stritem *next;

    u32 nbytes;         /* size of item data in bytes */
    u16 item_flags;     /* info for each item */
    u8 slabsclass_id;   /* which slabclass this item belongs to */

    u32 keylen;         /* key size in bytes, includes null termination */

    u64 data[];         /* actual data contained within the item */
} item;

typedef struct {
    u32 size;           /* item size */
    u32 perslab;        /* how many items per slab */

    void *slots;        /* list of free item ptrs */
    u32 slot_nfree;     /* total free items in list */

    u32 nslabs;          /* how many slabs were allocated for this class */

    void **slablist;    /* array of slab ptrs */
    u32 list_size;      /* previous list size */
} slabclass_t;

#define ITEM_data(it) ((char *)&((it)->data))
#define ITEM_key(it) ((char *)&((it)->data) + (it)->nbytes)
#define ITEM_ntotal(it) ((it)->keylen + (it)->nbytes + sizeof(item))

#define SLAB_dump(p) INFO("--SLAB-- size: %d, perslab: %d, slot_nfree: %d, nslabs: %d, list_size: %d", \
            (p)->size, (p)->perslab, (p)->slot_nfree, (p)->nslabs, (p)->list_size)

#define ITEM_dump(it) INFO("--ITEM--  nbytes: %d, item_flags: %x, slabclass_id: %d, keylen: %d, key: %s", \
                        it->nbytes, it->item_flags, it->slabsclass_id, it->keylen, ITEM_key(it))

#define ITEM_IN_SLAB 2
#define ITEM_HASHED 4
#define ITEM_HASH_REMOVED 8


/**
 * @param size size in bytes of whatever item you're using
 * @return The index/class_id for the slabclass that best fits the given size
 */
u32 slab_clsid(const u32 size);

/**
 * @param clsid The index/class_id of a slabclass
 * @return The item size in bytes for the given slabclass
 */
u32 slab_size(const u32 clsid);

void slabs_init();  /* initialize slab sizes */

void *slabs_alloc(const u32 size); /* returns an item struct */

void slabs_free(void *ptr);

slabclass_t *check_slabs(const u32 size);

slabclass_t *get_slabs();

void data_dump(void *item_in);

#endif /* SLAB_H */
