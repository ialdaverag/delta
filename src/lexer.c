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

static char advance(Lexer* lexer) {
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
                advance(lexer);
                break;
            default:
                return;
        }
    }
}

static bool is_hash(char c) {
    return c == '#';
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
    lexer->column++;
    
    return true;
}

static Token make_token(Lexer* lexer, TokenType type) {
    Token token;
    int length = (int)(lexer->current - lexer->start);

    Token_init(
        &token,
        type,
        lexer->start,
        lexer->current,
        lexer->line,
        lexer->column - length
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

static bool is_keyword(Lexer* lexer, int length, int offset, const char* keyword, int keyword_length) {
    return (lexer->current - lexer->start == length) && (memcmp(lexer->start + offset, keyword, keyword_length) == 0);
}

static TokenType identifier_type(Lexer* lexer) {
    int length = lexer->current - lexer->start;

    switch (lexer->start[0]) {
        case 'c':
            if (is_keyword(lexer, 4, 1, "aso", 3)) return TOKEN_CASO;
            if (is_keyword(lexer, 5, 1, "lase", 4)) return TOKEN_CLASE;
            if (is_keyword(lexer, 5, 1, "onst", 4)) return TOKEN_CONST;
            if (is_keyword(lexer, 9, 1, "ontinuar", 8)) return TOKEN_CONTINUAR;
            break;
        case 'f':
            if (is_keyword(lexer, 3, 1, "un", 2)) return TOKEN_FUN;
            if (is_keyword(lexer, 5, 1, "also", 4)) return TOKEN_FALSO;
            break;
        case 'm':
            if (is_keyword(lexer, 8, 1, "ientras", 7)) return TOKEN_MIENTRAS;
            break;
        case 'n':
            if (is_keyword(lexer, 2, 1, "o", 1)) return TOKEN_NO;
            if (is_keyword(lexer, 4, 1, "ulo", 3)) return TOKEN_NULO;
            break;
        case 'o':
            if (is_keyword(lexer, 4, 1, "tro", 3)) return TOKEN_OTRO;
            if (length == 1) return TOKEN_O;
            break;
        case 'p':
            if (is_keyword(lexer, 4, 1, "ara", 3)) return TOKEN_PARA;
            break;
        case 'r':
            if (is_keyword(lexer, 3, 1, "et", 2)) return TOKEN_RET;
            if (is_keyword(lexer, 6, 1, "omper", 5)) return TOKEN_ROMPER;
            break;
        case 's':
            if (is_keyword(lexer, 2, 1, "i", 1)) return TOKEN_SI;
            if (is_keyword(lexer, 4, 1, "ino", 3)) return TOKEN_SINO;
            if (is_keyword(lexer, 5, 1, "egun", 4)) return TOKEN_SEGUN;
            break;
        case 'v':
            if (is_keyword(lexer, 3, 1, "ar", 2)) return TOKEN_VAR;
            if (is_keyword(lexer, 9, 1, "erdadero", 8)) return TOKEN_VERDADERO;
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

static Token comment(Lexer* lexer) {
    while (peek(lexer) != '\n' && !is_at_end(lexer)) {
        advance(lexer);
    }

    return make_token(lexer, TOKEN_COMMENT);
}

static Token identifier(Lexer* lexer) {
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer)))
        advance(lexer);
    
    return make_token(lexer, identifier_type(lexer));
}

static Token number(Lexer* lexer) {
    const char* start = lexer->current - 1;
    bool has_decimal = false;

    while (is_digit(peek(lexer)))
        advance(lexer);

    if (peek(lexer) == '.' && is_digit(peek_next(lexer))) {
        has_decimal = true;
        advance(lexer);

        while (is_digit(peek(lexer)))
            advance(lexer);
    }

    if (peek(lexer) == '.') {
        lexer->current = start;

        return error_token(lexer, "ERROR: Numero mal formado.");
    }

    return make_token(lexer, has_decimal ? TOKEN_FLOAT : TOKEN_INTEGER);
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
    if (is_hash(c)) return comment(lexer);
    if (is_alpha(c)) return identifier(lexer);
    if (is_digit(c)) return number(lexer);
    if (is_quote(c)) return string(lexer);

    switch (c) {
        // One character tokens
        case '+': return make_token(lexer, TOKEN_PLUS);
        case '-': return make_token(lexer, TOKEN_MINUS);
        case '*': return make_token(lexer, TOKEN_STAR);
        case '/': return make_token(lexer, TOKEN_SLASH);
        case '%': return make_token(lexer, TOKEN_PERCENT);
        case '(': return make_token(lexer, TOKEN_LEFT_PARENTHESIS);
        case '[': return make_token(lexer, TOKEN_LEFT_BRACKET);
        case '{': return make_token(lexer, TOKEN_LEFT_BRACE);
        case ')': return make_token(lexer, TOKEN_RIGHT_PARENTHESIS);
        case ']': return make_token(lexer, TOKEN_RIGHT_BRACKET);
        case '}': return make_token(lexer, TOKEN_RIGHT_BRACE);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case '.': return make_token(lexer, TOKEN_DOT);
        case ':': return make_token(lexer, TOKEN_COLON);

        // Two character tokens
        case '=': return make_token(lexer, match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '!': return make_token(lexer, match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_ERROR);
        case '<': return make_token(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return make_token(lexer, match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    }

    return make_token(lexer, TOKEN_ERROR);
}