#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int toLower(char *);
int searchArray(char words[][6], int count, char[]);

int _main() {
    FILE *fptr;             // file pointer
    char words[5196][6];
    char searchWord[6];
    int result = 0;

    // open the file in read mode, check for success
    if ((fptr = fopen("wordlSorted.txt", "r")) == NULL) {
        printf("file could not be opened\n");
        exit(0);
    }

    // populate the array
    int index = 0;
    do {
        fscanf(fptr, "%s", words[index++]);
    } while (!(feof(fptr)));
    fclose(fptr);

    while (1) {
        printf("enter a word to search: ");
        scanf("%s", searchWord);

        toLower(searchWord);
        result = searchArray(words, 5196, searchWord);
        printf("the word %s was %s\n", searchWord, (result == 0) ? "not found" : "found");
    }
    return (0);
}
