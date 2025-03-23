#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* filename;
    char* content;
    long length;
} File;

void File_init(File* file, const char* filename);
void File_free(File* file);

#endif