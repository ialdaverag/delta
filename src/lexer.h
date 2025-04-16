#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <string.h> 
#include "token.h" 

#define MAXINDENT 100

typedef struct {
    const char* start;
    const char* current;
    int line;
    int column;
} Lexer;

void Lexer_init(Lexer* lexer, const char* source);
Token Lexer_next_token(Lexer* lexer);

#endif // LEXER_H