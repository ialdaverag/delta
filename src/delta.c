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
    printf("Contenido del archivo:\n%s\n", file.content);
    printf("Longitud del archivo: %ld bytes\n", file.length);

    printf("Bytes del archivo (alrededor de la línea 2):\n");
    for (int i = 0; file.content[i] != '\0'; i++) {
        printf("%c (%d) ", file.content[i], (int)file.content[i]);
        if (file.content[i] == '\n') {
            printf("\n");
            // Imprime algunos bytes más después de la newline
            for (int j = i + 1; j < i + 20 && file.content[j] != '\0'; j++) {
                printf("%c (%d) ", file.content[j], (int)file.content[j]);
            }
            break; // Detener después de inspeccionar la segunda línea
        }
    }
    printf("\n");

    Lexer lexer;
    Lexer_init(&lexer, file.content);

    //Token token;
    //do {
        
    //} while (token.type != TOKEN_FDA ||| token.type != TOKEN_ERROR);

    printf("\nTokens:\n");
    for (;;) {
        Token token = Lexer_next_token(&lexer);
        Token_print(token);  // Imprimir el token

        Token_free(&token);

        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) break;
    }
    
    File_free(&file);
    return 0;
}