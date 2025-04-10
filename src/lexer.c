#include "lexer.h"

void Lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;
}

static bool is_at_end(Lexer* lexer) {
    return *lexer->current == '\0';
}

char advance(Lexer* lexer) {
    char c = *lexer->current++;

    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    return c;
}

static char peek(Lexer* lexer) {
    return *lexer->current;
}

static char peek_next(Lexer* lexer) {
    if (is_at_end(lexer)) 
        return '\0';

    return lexer->current[1];
}

static void skip_whitespace(Lexer* lexer) {
    while (true) {
        char c = peek(lexer);

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                lexer->column = 1;
                advance(lexer);
                break;
            case '#':
                while (peek(lexer) != '\n' && !is_at_end(lexer)) {
                    advance(lexer);
                }
                break;
            default:
                return;
        }
    }
}

static bool is_eof(char c) {
    return c == '\0';
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
            c == '_';
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_quote(char c) {
    return c == '"' || c == '\'';
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
    Token_init(
        &token, 
        type, 
        lexer->start, 
        lexer->current, 
        lexer->line, 
        lexer->column - (int)(lexer->current - lexer->start)
    );

    return token;
}

static Token error_token(Lexer* lexer, const char* message) {
    Token token;
    Token_init(
        &token, 
        TOKEN_ERROR, 
        message, 
        message + strlen(message), 
        lexer->line, 
        lexer->column
    );

    return token;
}

static TokenType check_keyword(Lexer* lexer, int start, int length, const char* rest, TokenType type) {
    if (lexer->current - lexer->start == start + length && memcmp(lexer->start + start, rest, length) == 0)
        return type;

    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Lexer* lexer) {
    int length = lexer->current - lexer->start;

    switch (lexer->start[0]) {
        case 'c':
            if (length == 4 && memcmp(lexer->start + 1, "aso", 3) == 0) return TOKEN_CASO;
            if (length == 5 && memcmp(lexer->start + 1, "lase", 4) == 0) return TOKEN_CLASE;
            if (length == 5 && memcmp(lexer->start + 1, "onst", 4) == 0) return TOKEN_CONST;
            if (length == 9 && memcmp(lexer->start + 1, "ontinuar", 8) == 0) return TOKEN_CONTINUAR;
            break;
        case 'f':
            if (length == 3 && memcmp(lexer->start + 1, "un", 2) == 0) return TOKEN_FUN;
            if (length == 5 && memcmp(lexer->start + 1, "also", 4) == 0) return TOKEN_FALSO;
            break;
        case 'm':
            if (length == 8 && memcmp(lexer->start + 1, "ientras", 7) == 0) return TOKEN_MIENTRAS;
            break;
        case 'n':
            if (length == 2 && memcmp(lexer->start + 1, "o", 1) == 0) return TOKEN_NO;
            if (length == 4 && memcmp(lexer->start + 1, "ulo", 3) == 0) return TOKEN_NULO;
            break;
        case 'o':
            if (length == 4 && memcmp(lexer->start + 1, "tro", 3) == 0) return TOKEN_OTRO;
            if (length == 1) return TOKEN_O;
            break;
        case 'p':
            if (length == 4 && memcmp(lexer->start + 1, "ara", 3) == 0) return TOKEN_PARA;
            break;
        case 'r':
            if (length == 3 && memcmp(lexer->start + 1, "et", 2) == 0) return TOKEN_RET;
            if (length == 6 && memcmp(lexer->start + 1, "omper", 5) == 0) return TOKEN_ROMPER;
            break;
        case 's':
            if (length == 2 && memcmp(lexer->start + 1, "i", 1) == 0) return TOKEN_SI;
            if (length == 4 && memcmp(lexer->start + 1, "ino", 3) == 0) return TOKEN_SINO;
            if (length == 5 && memcmp(lexer->start + 1, "egun", 4) == 0) return TOKEN_SEGUN;
            break;
        case 'v':
            if (length == 3 && memcmp(lexer->start + 1, "ar", 2) == 0) return TOKEN_VAR;
            if (length == 9 && memcmp(lexer->start + 1, "erdadero", 8) == 0) return TOKEN_VERDADERO;
            break;
        case 'y':
            if (length == 1) return TOKEN_Y;
            break;
    }

    return TOKEN_IDENTIFIER;
}

static Token eof(Lexer* lexer) {
    return make_token(lexer, TOKEN_EOF);
}

static Token identifier(Lexer* lexer) {
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer)))
        advance(lexer);
    
    return make_token(lexer, identifier_type(lexer));
}

static Token number(Lexer* lexer) {
    while (is_digit(peek(lexer)))
        advance(lexer);
    
    if (peek(lexer) == '.' && is_digit(peek_next(lexer))) {
        advance(lexer);
        
        while (is_digit(peek(lexer)))
            advance(lexer);
        
        return make_token(lexer, TOKEN_FLOAT);
    }
    
    return make_token(lexer, TOKEN_INTEGER);
}

static Token string(Lexer* lexer) {
    char quote_type = lexer->current[-1];
    
    while (peek(lexer) != quote_type && !is_at_end(lexer)) {
        if (peek(lexer) == '\n') {
            advance(lexer);

            return error_token(lexer, "ERROR: Cadena no terminada.");
        }

        advance(lexer);
    }

    if (is_at_end(lexer))
        return error_token(lexer, "ERROR: Cadena no terminada.");

    advance(lexer);

    return make_token(lexer, TOKEN_STRING);
}

Token Lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    lexer->start = lexer->current;
    
    char c = advance(lexer);
    
    if (is_eof(c)) return eof(lexer);
    if (is_alpha(c)) return identifier(lexer);
    if (is_digit(c)) return number(lexer);
    if (is_quote(c)) return string(lexer);

    switch (c) {
        // One character tokens
        case '(': return make_token(lexer, TOKEN_LEFT_PARENTHESIS);
        case '[': return make_token(lexer, TOKEN_LEFT_BRACKET);
        case '{': return make_token(lexer, TOKEN_LEFT_BRACE);
        case ')': return make_token(lexer, TOKEN_RIGHT_PARENTHESIS);
        case ']': return make_token(lexer, TOKEN_RIGHT_BRACKET);
        case '}': return make_token(lexer, TOKEN_RIGHT_BRACE);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case '.': return make_token(lexer, TOKEN_DOT);
        case ':': return make_token(lexer, TOKEN_COLON);
        case '+': return make_token(lexer, TOKEN_PLUS);
        case '-': return make_token(lexer, TOKEN_MINUS);
        case '*': return make_token(lexer, TOKEN_STAR);
        case '/': return make_token(lexer, TOKEN_SLASH);
        case '%': return make_token(lexer, TOKEN_PERCENT);

        // Two character tokens
        case '=': return make_token(lexer, match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '!': return make_token(lexer, match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_UNKNOWN);
        case '<': return make_token(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return make_token(lexer, match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    }

    return make_token(lexer, TOKEN_UNKNOWN);
}