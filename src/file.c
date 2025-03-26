#include "file.h"

void File_init(File* file, const char* filename) {
    file->filename = strdup(filename);

    FILE* fp = fopen(filename, "rb");

    // Verifica si el archivo se pudo abrir
    if (fp == NULL) {
        fprintf(stderr, "Error: No se puede abrir el archivo '%s'\n", filename);
        File_free(file);

        return;
    }

    fseek(fp, 0, SEEK_END);
    file->length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    file->content = (char*)malloc(file->length + 1);

    fread(file->content, 1, file->length, fp);
    file->content[file->length] = '\0';

    fclose(fp);
}

void File_free(File* file) {
    if (file->filename != NULL) {
        free(file->filename);
        file->filename = NULL;
    }

    if (file->content != NULL) {
        free(file->content);
        file->content = NULL;
    }

    file->length = 0;
}