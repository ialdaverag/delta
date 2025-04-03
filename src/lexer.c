#include "lexer.h"

void Lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

static bool is_at_end(Lexer* lexer) {
    return *lexer->current == '\0';
}

static char advance(Lexer* lexer) {
    lexer->current++;

    return lexer->current[-1];
}

static char peek(Lexer* lexer) {
    return *lexer->current;
}

static void skip_whitespace(Lexer* lexer) {
    for (;;) {
        char c = peek(lexer);

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                advance(lexer);
                break;
            default:
                return;
        }
    }
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
    skip_whitespace(lexer);

    lexer->start = lexer->current;

    if (is_at_end(lexer)) 
        return make_token(lexer, TOKEN_FDA);

    char c = advance(lexer);

    switch (c) {
        case '(':
            return make_token(lexer, TOKEN_PARENTESIS_IZQUIERDO);
        case ')':
            return make_token(lexer, TOKEN_PARENTESIS_DERECHO);
        case '[':
            return make_token(lexer, TOKEN_CORCHETE_IZQUIERDO);
        case ']':
            return make_token(lexer, TOKEN_CORCHETE_DERECHO);
        case '{':
            return make_token(lexer, TOKEN_LLAVE_IZQUIERDA);
        case '}':
            return make_token(lexer, TOKEN_LLAVE_DERECHA);
        case ',':
            return make_token(lexer, TOKEN_COMA);
        case '.':
            return make_token(lexer, TOKEN_PUNTO);
        case ':':
            return make_token(lexer, TOKEN_DOS_PUNTOS);
    }

    return error_token(lexer, "Caracter inesperado.");
}