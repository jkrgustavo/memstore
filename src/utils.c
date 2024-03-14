#define UTIL_STD
#include "utils.h"

char **process_words(char *buffer) {
    char **new_buffer = (char**)malloc(sizeof(char*) * 5196);
    char *token;
    int index = 0;

    token = strtok(buffer, "\r\n");

    while (token != NULL) {
        new_buffer[index] = token;
        token = strtok(NULL, "\r\n");
        index++;
    }

    return new_buffer;
}

char *read_file(const char *file_name) {
    FILE *fptr;

    if ((fptr = fopen(file_name, "r")) == NULL) {
        ERROR("Couldn't open %s", file_name);
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    int file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char *buffer = (char*)malloc(file_size + 1);
    fread(buffer, 1, file_size, fptr);
    buffer[file_size] = '\0';

    fclose(fptr);
    return buffer;
}
