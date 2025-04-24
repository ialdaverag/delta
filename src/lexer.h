#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <string.h> 
#include <stdlib.h>
#include "token.h" 

#define MAX_PARENT_STACK 100

typedef struct {
    const char* start;
    const char* current;
    int line;
    int column;

    int token_line;
    int token_column;

    bool saw_token;

    int parent_level;
    int parent_stack[MAX_PARENT_STACK];
    int parent_line_stack[MAX_PARENT_STACK];
    int parent_column_stack[MAX_PARENT_STACK];
} Lexer;

void Lexer_init(Lexer* lexer, const char* source);
Token Lexer_next_token(Lexer* lexer);

#endif // LEXER_H