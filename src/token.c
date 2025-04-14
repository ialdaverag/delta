#include "token.h"
#include <stdlib.h>
#include <string.h>

Token Token_init(Token* token, TokenType type, const char* start, const char* current, int line, int column) {
    token->type = type;
    token->line = line;
    token->column = column;

    token->lexeme = (char*)malloc((current - start) + 1);

    if (token->lexeme != NULL) {
        strncpy(token->lexeme, start, current - start);
        token->lexeme[current - start] = '\0'; 
    }

    return *token;
}

void Token_free(Token* token) {
    if (token->lexeme != NULL) {
        free(token->lexeme);
        token->lexeme = NULL;
    }
}

Token Token_print(Token token) {
    printf(
        "Token [L%d:C%d]: %d, %s\n", 
        token.line, 
        token.column, 
        token.type, 
        token.lexeme
    );

    return token;
}