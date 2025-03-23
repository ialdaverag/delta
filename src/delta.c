#include <stdio.h>

#include "file.h"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        
        return 1;
    }

    const char* filename = argv[1];

    File file;
    File_init(&file, filename);

    printf("Nombre del archivo: %s\n", file.filename);
    printf("Contenido del archivo:\n%s\n", file.content);
    printf("Longitud del archivo: %ld bytes\n", file.length);

    File_free(&file);

    return 0;
}