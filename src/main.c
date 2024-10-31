#include "hash.h"
#include "slab.h"
#define UTIL_ALL
#include "utils.h"

typedef struct {
    s32 number_data;
    char word[];
} foo;

item *word_data[1200];

int main() {
    char *file_name = "wordlSorted.txt";
    char *buffer = read_file(file_name);
    char **words = process_words(buffer);
    slabs_init();

    for (int i = 0; i < 1200; i++) {
        word_data[i] = slabs_alloc(sizeof(foo) + 7);
        foo *data = (foo*)ITEM_data(word_data[i]);
        data->number_data = i*100000;
        strncpy(data->word, words[i * 2], 6);
        hash_insert(words[i], word_data[i]);
    }

    for (int i = 0; i < 1200; i++) {
        item *a = hash_retrieve(words[i]);
        item *b = word_data[i];
        if (strcmp(ITEM_key(a), ITEM_key(b))) {
            ERROR("%s", "oop");
        }
        
    }

    free(words);
    free(buffer);
    hash_destroy();
}
