#include "slab.h"

#define MAX_NUMBER_OF_SLAB_CLASSES 3
#define POWER_SMALLEST 1
#define POWER_LARGEST MAX_NUMBER_OF_SLAB_CLASSES - POWER_SMALLEST
#define GROWTH_FACTOR 1.25

#define SLAB_PAGE_SIZE 1000 //1024 * 1024 /* 1MB page size */

#define CHUNK_SIZE 48
#define CHUNK_ALIGN 8



static slabclass_t slabclass[MAX_NUMBER_OF_SLAB_CLASSES];


/* Finds the right slabclass for a given size
 *
 * 0 is an error, either invalid size or too big
 */
u32 slab_clsid(const u32 size) {
    u32 index = 0;

    if (size == 0)
        return 0;
    while (size + sizeof(item) > slabclass[index].size)
        if (index++ == MAX_NUMBER_OF_SLAB_CLASSES)       /* wont fit in the biggest slab */
            return 0;

    return index;
}

u32 slab_size(const u32 id) {
    return slabclass[id].size;
}

void slabs_init() {
    u32 i = POWER_SMALLEST - 1;
    u32 size = sizeof(item) + CHUNK_SIZE;

    memset(slabclass, 0, sizeof(slabclass));

    while (i++ <= POWER_LARGEST) {
        /* align chunk sizes */
        if (size % CHUNK_ALIGN) size += CHUNK_ALIGN - (size % CHUNK_ALIGN);

        slabclass[i].size = size;
        slabclass[i].perslab = SLAB_PAGE_SIZE / slabclass[i].size;
        size *= GROWTH_FACTOR;
        INFO("slabclass: %d,    size: %d,   perslab: %d", i, slabclass[i].size, slabclass[i].perslab);
    }
}

static void do_slabs_free(void *ptr, u32 id) {
    slabclass_t *p;
    item *it;
    assert(id <= MAX_NUMBER_OF_SLAB_CLASSES && id >= POWER_SMALLEST);
    memset(ptr, 0, sizeof(item) + it->nbytes);
    
    p = &slabclass[id];
    it = (item *)ptr;

    it->next = p->slots;
    it->prev = 0;
    if (it->next) it->next->prev = it;
    p->slots = it;
    p->slot_nfree++;
    return;
}

static int grow_slab_list(const u32 id) {
    slabclass_t *p = &slabclass[id];
    u32 new_size = (p->nslabs == 0) ? 4 : p->list_size * 2;
    void *new_list = realloc(p->slablist, new_size * sizeof(void *));
    if (!new_list) return 0;
    p->list_size = new_size;
    p->slablist = new_list;
    return 1;
}

static void split_slab_into_freelist(char *ptr, u32 id) {
    slabclass_t *p = &slabclass[id];
    for (int i = 0; i < p->perslab; i++) {
        do_slabs_free(ptr, id);
        ptr += p->size;
    }
}

static int do_slabs_newslab(const u32 id) {
    slabclass_t *p = &slabclass[id];
    u32 len = p->size * p->perslab;
    char *ptr;

    if (grow_slab_list(id) == 0) {
        ERROR("%s", "Newslab creation failed");
        return 0;
    }

    ptr = malloc(len);
    memset(ptr, 0, (size_t)len);
    split_slab_into_freelist(ptr, id);
    p->slablist[p->nslabs++] = ptr;

    return 1;
}

static int item_init(void *ptr, const u32 size, const u32 id) {
    item *it = (item *)ptr;
    it->next = it->prev = 0;
    it->nbytes = size;
    it->slabsclass_id = id;
    return 1;
}

/* can return null */
void *slabs_alloc(const u32 size) {
    slabclass_t *p;
    u32 id = slab_clsid(size);
    item *it = NULL;
    void *ret = NULL;
   
    if (id == 0) {
        ERROR("Slabclass allocation failed%s", "");
        ret = NULL;
    }
    p = &slabclass[id];
    
    if (p->slot_nfree == 0 && !do_slabs_newslab(id)) {
        ERROR("%s", "Unable to create newslab");
        ret = NULL;
    }

    it = (item*)p->slots;
    p->slots = it->next;
    if (it->next) it->next->prev = 0;
    p->slot_nfree--;
    item_init(it, size, id);

    ret = (void*)it;


    DEBUG("==ALLOC== size: %d, class: %d, item size: %d, address: %p", size, id, p->size, ret);
    return ret;
}

void slabs_free(void *ptr) {
    item *it = (item *)ptr;
    do_slabs_free(ptr, slab_clsid(it->slabsclass_id));
}

slabclass_t *check_slabs(const u32 size) {
    return &slabclass[slab_clsid(size)];
}

slabclass_t *get_slabs() {
    return slabclass;
}
