#include "token.h"

Token Token_init(Token* token, TokenType type, const char* start, const char* current, int line) {
    token->type = type;
    token->start = start;
    token->length = (int)(current - start);
    token->line = line;
    
    return *token;
}