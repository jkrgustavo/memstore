#define UTIL_ALL
#include "utils.h"
#include "hash.h"
#include "slab.h"

static HashMap *map = {0};

typedef unsigned long int u4;
typedef unsigned char u1;

#define LOAD_BALANCE 1

/* The mixing step */
#define mix64(a,b,c) \
{ \
  a=a-b;  a=a-c;  a=a^(c>>43); \
  b=b-c;  b=b-a;  b=b^(a<<9);  \
  c=c-a;  c=c-b;  c=c^(b>>8);  \
  a=a-b;  a=a-c;  a=a^(c>>38); \
  b=b-c;  b=b-a;  b=b^(a<<23); \
  c=c-a;  c=c-b;  c=c^(b>>5);  \
  a=a-b;  a=a-c;  a=a^(c>>35); \
  b=b-c;  b=b-a;  b=b^(a<<49); \
  c=c-a;  c=c-b;  c=c^(b>>11); \
  a=a-b;  a=a-c;  a=a^(c>>12); \
  b=b-c;  b=b-a;  b=b^(a<<18); \
  c=c-a;  c=c-b;  c=c^(b>>22); \
}

/* The whole new hash function */
u4 hash( register u1 *k, u4 length, u4 initval)
//register u1 *k;        /* the key */
//u4           length;   /* the length of the key in bytes */
//u4           initval;  /* the previous hash, or an arbitrary value */
{
   register u4 a,b,c;  /* the internal state */
   u4          len;    /* how many key bytes still need mixing */

   /* Set up the internal state */
   len = length;
   a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
   c = initval;         /* variable initialization of internal state */

   /*---------------------------------------- handle most of the key */
   while (len >= 12)
   {
      a=a+(k[0]+((u4)k[1]<<8)+((u4)k[2]<<16) +((u4)k[3]<<24));
      b=b+(k[4]+((u4)k[5]<<8)+((u4)k[6]<<16) +((u4)k[7]<<24));
      c=c+(k[8]+((u4)k[9]<<8)+((u4)k[10]<<16)+((u4)k[11]<<24));
      mix64(a,b,c);
      k = k+12; len = len-12;
   }

   /*------------------------------------- handle the last 11 bytes */
   c = c+length;
   switch(len)              /* all the case statements fall through */
   {
   case 11: c=c+((u4)k[10]<<24);
   case 10: c=c+((u4)k[9]<<16);
   case 9 : c=c+((u4)k[8]<<8);
      /* the first byte of c is reserved for the length */
   case 8 : b=b+((u4)k[7]<<24);
   case 7 : b=b+((u4)k[6]<<16);
   case 6 : b=b+((u4)k[5]<<8);
   case 5 : b=b+k[4];
   case 4 : a=a+((u4)k[3]<<24);
   case 3 : a=a+((u4)k[2]<<16);
   case 2 : a=a+((u4)k[1]<<8);
   case 1 : a=a+k[0];
     /* case 0: nothing left to add */
   }
   mix64(a,b,c);
   /*-------------------------------------------- report the result */
   return c;
}

int next_size(int min) {
    return min * 2;
}

HashMap *hash_create(int size) {
    if (size < 0) {
        fprintf(stderr, "Invalid hashmap size: %s\n", __func__);
        return NULL;
    }


    HashMap *new = malloc(sizeof(HashMap)); 
    new->table = malloc(sizeof(item *) * size);
    memset(new->table, 0, sizeof(item *) * size);
    new->size = size;
    new->length = 0;
    return new;
}

// state = 0: insert, key == NULL    state = 1: search      state = 2: delete
static int quadratic_probe(HashMap *map, unsigned long start, int state, const char *key) {
    unsigned long index = start;
    int probe_count = 0;
    int inc = 0;

    float c1 = 1;
    float c2 = 0.7;

    if (state == 0) {
        while (map->table[index] != 0 && probe_count <= map->size * 2) {
            index = (int)(index + c1 * inc + c2 * inc * inc) & (map->size - 1);
            inc++;
            probe_count++;
        }
        if (probe_count >= map->size * 2) {
            fprintf(stderr, "Max insert reached: %s\n", __func__);
            return -1;
        }
    } else if (state == 1 && key != NULL) {
        while (map->table[index] != NULL && probe_count <= map->size * 2) {
            if (map->table[index] != NULL && strcmp(ITEM_key(map->table[index]), key) == 0) { break; }

            index = (int)(index + c1 * inc + c2 * inc * inc) & (map->size - 1);
            inc++;
            probe_count++;           
        }

        if (probe_count >= map->size * 2) {
            fprintf(stderr, "Max search reached: %s\n", __func__);
            return -1;
        }
        
    } else if (state == 2 && key != NULL) {
        while (map->table[index] != 0 && probe_count <= map->size * 2) {
            if (map->table[index] != 0 && strcmp(ITEM_key(map->table[index]), key) == 0) { break; }

            index = (int)(index + c1 * inc + c2 * inc * inc) & (map->size - 1);
            inc++;
            probe_count++;           
        }

        if (probe_count >= map->size * 2) {
            fprintf(stderr, "Max delete reached: %s\n", __func__);
            return -1;
        }
    } else {
        fprintf(stderr, "Invalid state, only 0 (insert), 1 (search), 2 (delete). Given: %d in %s", state, __func__);
        return -1;
    }


    return index;
}

// Resizes hashmap
static void hash_resize() {
    HashMap *new = hash_create(next_size(map->size));

    for (int i = 0; i < map->size; i++) {
        item *it = map->table[i];
        if (it != NULL) {
            unsigned long index = hash((unsigned char*)ITEM_key(map->table[i]), map->table[i]->keylen, 0) & new->size-1;
            index = quadratic_probe(new, index, 0, NULL);

            new->table[index] = map->table[i];
            new->length++;
        } else {
            fprintf(stderr, "Non-full resize: %s\n", __func__);
        }
    }

    //free(map->table);
    free(map);
    map = new;
}

void hash_insert(const char *key, void *data) {
    if (map == NULL) {
        slabs_init();
        map = hash_create(128);
    } else if (map->size * LOAD_BALANCE <= map->length) {
        hash_resize();
    }

    u32 key_length = strnlen(key, 100);
    if (key_length >= 100) {
        fprintf(stderr, "Error finding key length. key: %s, %s\n", key, __func__);
    }


    item *it = data;
    if (it->item_flags != 0)
        fprintf(stderr, "Error allocating item: %s\n", __func__);

    it->keylen = key_length;

    it->item_flags |= ITEM_HASHED;
    memcpy(ITEM_key(it), key, it->keylen);

    unsigned long index = hash((unsigned char*)ITEM_key(it), it->keylen, 0) & map->size-1;
    index = quadratic_probe(map, index, 0, NULL);

    if ((map->table[index] != NULL) || index == -1) {
        fprintf(stderr, "Attempt to insert into invalid slot, key: %s\n", key);
        return;
    }
    
    map->table[index] = it;
    map->length++;
}

void *hash_retrieve(const char *key) {
    long index = hash((unsigned char*)key, strnlen(key, 100), 0) & map->size-1;

    index = quadratic_probe(map, index, 1, key);



    if (index == -1 || map->table[index] == NULL) {
        fprintf(stderr, "Attempt to retrieve from invalid slot: %s\n", __func__);
        return NULL;
    }

    item *it = map->table[index];
    return it;
}

void hash_delete(const char *key) {
    if (map->length <= 0) {
        fprintf(stderr, "Cannot delete from empty table! key: %s", key);
    }

    long index = hash((unsigned char *)key, strnlen(key, 20), 0) & map->size-1;
    index = quadratic_probe(map, index, 2, key);
    item *it = map->table[index];

    if (index == -1 || it->item_flags & ITEM_HASHED) {
        fprintf(stderr, "Attempt to delete from invalid slot, key: %s\n", key);
    }

    slabs_free(it);
    map->table[index] = NULL;
    map->length--;
} 

void hash_destroy() {
    if (map == NULL)
        return;
    DEBUG("(Final info) size: %d, length: %d", map->size, map->length);

    for (int i = 0; i < map->size; i++) {
        item *it = map->table[i];
        if (it != NULL) {
            slabs_free(it);
        }
    }
    free(map->table);
    free(map);
}

int hash_status(int index) {
    if (index < 0 || index >= map->size) {
        fprintf(stderr, "Invalid index: %s\n", __func__);
        return -1;
    }

    item *it = map->table[index];

    if (it == NULL)
        return 0;

    return 1;
}

int hash_size() {
    return map->size;
}

