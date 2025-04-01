#include "file.h"
#include "token.h"

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

    const char* start = "var";
    const char* current = start + 3; // Longitud de la cadena "var"
    int line = 1;

    Token token;
    Token_init(&token, TOKEN_VAR, start, current, line);

    printf("Token: %d\n", token.type);
    printf("Inicio del token: %s\n", token.start);
    printf("Longitud del token: %d\n", token.length);
    printf("Linea del token: %d\n", token.line);

    
    File_free(&file);

    return 0;
}