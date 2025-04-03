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

static bool match(Lexer* lexer, char expected) {
    if (is_at_end(lexer)) 
        return false;

    if (*lexer->current != expected) 
        return false;

    lexer->current++;
    
    return true;
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
        // Delimitadores
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

        // Operador de asignación
        case '=':
            if (match(lexer, '=')) 
                return make_token(lexer, TOKEN_IGUAL);
            
            return make_token(lexer, TOKEN_ASIGNACION);

        // Operadores aritméticos
        case '+':
            return make_token(lexer, TOKEN_MAS);
        case '-':
            return make_token(lexer, TOKEN_MENOS);
        case '*':
            return make_token(lexer, TOKEN_ASTERISCO);
        case '/':
            return make_token(lexer, TOKEN_DIAGONAL);
        case '%':
            return make_token(lexer, TOKEN_PORCENTAJE);

        // Operadores de comparación
        case '!':
            if (match(lexer, '=')) 
                return make_token(lexer, TOKEN_DISTINTO);

            break;
        case '<':
            if (match(lexer, '=')) 
                return make_token(lexer, TOKEN_MENOR_IGUAL);

            return make_token(lexer, TOKEN_MENOR);
        case '>':
            if (match(lexer, '=')) 
                return make_token(lexer, TOKEN_MAYOR_IGUAL);

            return make_token(lexer, TOKEN_MAYOR);
    }

    return error_token(lexer, "Caracter inesperado.");
}