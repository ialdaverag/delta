#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <string.h> 
#include <stdlib.h>
#include "token.h" 

#define MAX_PARENT_STACK 100
#define MAX_INDENT_STACK 100
#define TAB_SIZE 4

typedef struct {
    // Inicio y fin del texto fuente
    const char* start;
    const char* current;

    // Línea y columna del texto fuente
    int line;
    int column;

    // Línea y columna del token actual	
    int token_line;
    int token_column;

    // Token significativo
    bool saw_token;

    // Indentación
    int indent_stack[MAX_INDENT_STACK];
    int indent_top;
    int pending;
    bool atbol;

    // Delimitadores
    int parent_stack[MAX_PARENT_STACK];
    int parent_top;
    int parent_line_stack[MAX_PARENT_STACK];
    int parent_column_stack[MAX_PARENT_STACK];
} Lexer;

void Lexer_init(Lexer* lexer, const char* source);
Token Lexer_next_token(Lexer* lexer);

#endif // LEXER_H