#include "file.h"
#include "lexer.h"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        
        return 1;
    }

    const char* filename = argv[1];

    File file;
    File_init(&file, filename);

    printf("Nombre del archivo: %s\n", file.filename);
    printf("Longitud del archivo: %ld bytes\n", file.length);
    printf("Contenido del archivo:\n%s\n", file.content);

    Lexer lexer;
    Lexer_init(&lexer, file.content);

    printf("\nTokens:\n");
    for (;;) {
        Token token = Lexer_next_token(&lexer);
        Token_print(token);

        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            Token_free(&token);
            break;
        }
        
        Token_free(&token);
    }
    
    File_free(&file);
    return 0;
}