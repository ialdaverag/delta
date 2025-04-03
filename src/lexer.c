#include "lexer.h"

void Lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

static bool is_at_end(Lexer* lexer) {
    return *lexer->current == '\0';
}

static Token make_token(Lexer* lexer, TokenType type) {
    Token token;
    Token_init(&token, type, lexer->start, lexer->current, lexer->line);

    return token;
}

static Token error_token(Lexer* lexer, const char* message) {
    Token token;
    Token_init(&token, TOKEN_ERROR, message, message + strlen(message), lexer->line);

    return token;
}

Token Lexer_next_token(Lexer* lexer) {
    lexer->start = lexer->current;

    if (is_at_end(lexer)) 
        return make_token(lexer, TOKEN_FDA);

    return error_token(lexer, "Caracter inesperado.");
}