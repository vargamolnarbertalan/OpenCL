#include "kernel_loader.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char* read_kernel_source(const char* filename) {
    FILE* file;
    long size;
    char* buffer;

    file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    buffer = (char*)malloc(sizeof(char) * (size + 1));
    if (!buffer) {
        fprintf(stderr, "Error: Failed to allocate memory for buffer\n");
        fclose(file);
        return NULL;
    }

    if (fread(buffer, sizeof(char), size, file) != size) {
        fprintf(stderr, "Error: Failed to read file '%s'\n", filename);
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[size] = '\0';

    fclose(file);

    return buffer;
}