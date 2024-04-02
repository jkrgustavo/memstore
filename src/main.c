#include "hash.h"
#include "slab.h"
#define UTIL_ALL
#include "utils.h"

typedef struct {
    s32 number_data;
    char *word;
} foo;

void slab_dump(slabclass_t *slab) {
    INFO("--SLAB-- size: %d, perslab: %d, slot_nfree: %d, nslabs: %d, list_size: %d",
            slab->size, slab->perslab, slab->slot_nfree, slab->nslabs, slab->list_size);
}

void item_dump(item *it, u32 i) {
    foo *item = ITEM_data(it, foo);
    INFO("--ITEM %d--  nbytes: %d, item_flags: %d, slabclass_id: %d, num: %d",
            i, it->nbytes, it->item_flags, it->slabsclass_id, item->number_data);
}

void iterate_item_list() {
    slabclass_t *slabclasses = get_slabs();
    slabclass_t *p = &slabclasses[1];
    slab_dump(p);
    item *it = p->slots;
    void *ptr = p->slablist[0];


    for (u32 i = 0; i < p->perslab; i++) {
        item_dump(ptr, i);
        ptr += p->size;
    }

}

int main() {
    char *file_name = "wordlSorted.txt";
    char *buffer = read_file(file_name);
    char **words = process_words(buffer);
    slabs_init();
    item *it[13];

    for (int i = 0; i < 13; i++) {
        it[i] = slabs_alloc(sizeof(foo));
        *ITEM_data(it[i], foo) = (foo){ .number_data = i, .word = "hey there"};
    }

    iterate_item_list();
    LOG("----------------------\n");

    for (int i = 0; i < 13; i++) {
        slabs_free(it[i]);
    }

    iterate_item_list();

    free(words);
    free(buffer);
    hash_destroy();
}
