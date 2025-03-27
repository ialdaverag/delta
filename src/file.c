#include "file.h"

void File_init(File* file, const char* filename) {
    // Inicializa la estructura
    file->filename = strdup(filename);

    // Intenta abrir el archivo
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: No se puede abrir el archivo '%s'\n", filename);
        File_free(file);

        return;
    }

    // Obtiene la longitud del archivo
    fseek(fp, 0, SEEK_END);
    file->length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Reserva memoria para el contenido del archivo
    file->content = (char*)malloc(file->length + 1);
    if (file->content == NULL) {
        fprintf(stderr, "Error: No se puede reservar memoria para el archivo '%s'\n", filename);
        File_free(file);
        fclose(fp);

        return;
    }

    // Lee el contenido del archivo en el buffer
    size_t bytes_read = fread(file->content, 1, file->length, fp);
    if (bytes_read != file->length) {
        fprintf(stderr, "Error: Unable to read file '%s'. %s\n", filename, strerror(errno));
        free(file->content);
        fclose(fp);

        return;
    }

    // Agrega el caracter nulo al final del contenido
    file->content[file->length] = '\0';

    // Cierra el archivo
    fclose(fp);
}

void File_free(File* file) {
    // Libera la memoria reservada para el nombre del archivo
    if (file->filename != NULL) {
        free(file->filename);
        file->filename = NULL;
    }

    // Libera la memoria reservada para el contenido del archivo
    if (file->content != NULL) {
        free(file->content);
        file->content = NULL;
    }

    // Establece la longitud del archivo en 0
    file->length = 0;
}