#include "lexer.h"

void Lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;

    lexer->token_line = 1;
    lexer->token_column = 1;

    lexer->parent_level = 0;
    lexer->saw_token = false;
}

static bool is_at_end(Lexer* lexer) {
    return *lexer->current == '\0';
}

static char peek(Lexer* lexer) {
    return *lexer->current;
}

static char peek_next(Lexer* lexer) {
    if (is_at_end(lexer)) {
        return '\0';
    }

    return lexer->current[1];
}

static char advance(Lexer* lexer) {
    char c = *lexer->current++;

    if (c == '\r') {
        if (*lexer->current == '\n') {
            lexer->current++;
        }

        lexer->line++;
        lexer->column = 1;

        return '\n';
    }

    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;

        return '\n';
    }
        
    lexer->column++;

    return c;
}

static void skip_whitespace(Lexer* lexer) {
    while (true) {
        char c = peek(lexer);

        switch (c) {
            case ' ':
                advance(lexer);
                break;
            case '\t':
                advance(lexer);
                break;
            default:
                return;
        }
    }
}

static bool match(Lexer* lexer, char expected) {
    if (is_at_end(lexer)) {
        return false;
    }

    if (*lexer->current != expected) {
        return false;
    }

    lexer->current++;
    lexer->column++;
    
    return true;
}

static bool is_hash(char c) {
    return c == '#';
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
    return c == '\'' || c == '"' ;
}

static bool is_eof(char c) {
    return c == '\0';
}

static bool is_newline(char c) {
    return c == '\n' || c == '\r';  // Considera ambos como newline
}

static bool is_keyword(Lexer* lexer, int length, int offset, const char* keyword, int keyword_length) {
    return (lexer->current - lexer->start == length) && 
           (memcmp(lexer->start + offset, keyword, keyword_length) == 0);
}

static TokenType identifier_type(Lexer* lexer) {
    int length = lexer->current - lexer->start;

    switch (lexer->start[0]) {
        case 'c':
            // caso
            if (is_keyword(lexer, 4, 1, "aso", 3)) {
                return TOKEN_CASO;
            }

            // clase
            if (is_keyword(lexer, 5, 1, "lase", 4)) {
                return TOKEN_CLASE;
            }

            // constante
            if (is_keyword(lexer, 5, 1, "onst", 4)) {
                return TOKEN_CONST;
            }

            // continuar
            if (is_keyword(lexer, 9, 1, "ontinuar", 8)) {
                return TOKEN_CONTINUAR;
            }

            break;
        case 'f':
            // fun
            if (is_keyword(lexer, 3, 1, "un", 2)) {
                return TOKEN_FUN;
            }

            // falso
            if (is_keyword(lexer, 5, 1, "also", 4)) {
                return TOKEN_FALSO;
            }

            break;
        case 'm':
            // mientras
            if (is_keyword(lexer, 8, 1, "ientras", 7)) {
                return TOKEN_MIENTRAS;
            }

            break;
        case 'n':
            // no
            if (is_keyword(lexer, 2, 1, "o", 1)) {
                return TOKEN_NO;
            }

            // nulo
            if (is_keyword(lexer, 4, 1, "ulo", 3)) {
                return TOKEN_NULO;
            }

            break;
        case 'o':
            // otro
            if (is_keyword(lexer, 4, 1, "tro", 3)) {
                return TOKEN_OTRO;
            }

            // o
            if (length == 1) {
                return TOKEN_O;
            }

            break;
        case 'p':
            // para
            if (is_keyword(lexer, 4, 1, "ara", 3)) {
                return TOKEN_PARA;
            }

            break;
        case 'r':
            // ret
            if (is_keyword(lexer, 3, 1, "et", 2)) {
                return TOKEN_RET;
            }

            // romper
            if (is_keyword(lexer, 6, 1, "omper", 5)) {
                return TOKEN_ROMPER;
            }

            break;
        case 's':
            // si
            if (is_keyword(lexer, 2, 1, "i", 1)) {
                return TOKEN_SI;
            }

            // sino
            if (is_keyword(lexer, 4, 1, "ino", 3)) {
                return TOKEN_SINO;
            }
            
            // segun
            if (is_keyword(lexer, 5, 1, "egun", 4)) {
                return TOKEN_SEGUN;
            }

            break;
        case 'v':
            // var
            if (is_keyword(lexer, 3, 1, "ar", 2)) {
                return TOKEN_VAR;
            }

            // valor
            if (is_keyword(lexer, 9, 1, "erdadero", 8)) {
                return TOKEN_VERDADERO;
            }

            break;
        case 'y':
            if (length == 1) {
                return TOKEN_Y;
            }

            break;
    }

    return TOKEN_IDENTIFIER;
}

static Token make_token(Lexer* lexer, TokenType type) {
    // Marcar token como significativo
    if (type != TOKEN_COMMENT && 
        type != TOKEN_NEWLINE && 
        type != TOKEN_NL && 
        type != TOKEN_EOF && 
        type != TOKEN_ERROR) {
        lexer->saw_token = true;
    }

    Token token;
    Token_init(
        &token,
        type,
        lexer->start,
        lexer->current,
        lexer->token_line,
        lexer->token_column
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
        lexer->token_line,
        lexer->token_column
    );

    return token;
}

static Token eof(Lexer* lexer) {
    return make_token(lexer, TOKEN_EOF);
}

static Token comment(Lexer* lexer) {
    // 1. Consumir todo el comentario (excepto el \n)
    while (!is_at_end(lexer) && !is_newline(peek(lexer))) {
        advance(lexer);
    }

    return make_token(lexer, TOKEN_COMMENT);
}

static Token identifier(Lexer* lexer) {
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer))) {
        advance(lexer);
    }
    
    return make_token(lexer, identifier_type(lexer));
}

static Token number(Lexer* lexer) {
    const char* start = lexer->current - 1;
    bool has_decimal = false;

    while (is_digit(peek(lexer))) {
        advance(lexer);
    }

    if (peek(lexer) == '.' && is_digit(peek_next(lexer))) {
        has_decimal = true;
        advance(lexer);

        while (is_digit(peek(lexer))) {
            advance(lexer);
        }
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

    if (is_at_end(lexer)) {
        return error_token(lexer, "ERROR: Cadena no terminada.");
    }

    advance(lexer);

    return make_token(lexer, TOKEN_STRING);
}

Token Lexer_next_token(Lexer* lexer) {
    // Esquivar espacios en blanco
    skip_whitespace(lexer);

    // Guardar el inicio del token
    lexer->start = lexer->current;
    lexer->token_line = lexer->line;
    lexer->token_column = lexer->column;

    // Avanzar al siguiente caracter
    char c = advance(lexer);

    // Nueva línea
    if (is_newline(c)) {
        Token token;
    
        if (lexer->parent_level > 0 || !lexer->saw_token) {
            token = make_token(lexer, TOKEN_NL);
        } else {
            token = make_token(lexer, TOKEN_NEWLINE);
        }
    
        lexer->saw_token = false;
        
        return token;
    }
    
    // EOF
    if (is_eof(c)) {
        return eof(lexer);
    }

    // Comentario
    if (is_hash(c)) {
        return comment(lexer);
    }

    // Identificador o palabra clave
    if (is_alpha(c)) {
        return identifier(lexer);
    }

    // Numero
    if (is_digit(c)) {
        return number(lexer);
    }

    // Cadena
    if (is_quote(c)) {
        return string(lexer);
    }

    // Tokens de uno o dos caracteres
    switch (c) {
        // Token de un caracter
        case '+': 
            return make_token(lexer, TOKEN_PLUS);
        case '-': 
            return make_token(lexer, TOKEN_MINUS);
        case '*': 
            return make_token(lexer, TOKEN_STAR);
        case '/': 
            return make_token(lexer, TOKEN_SLASH);
        case '%': 
            return make_token(lexer, TOKEN_PERCENT);
        case '(': 
            lexer->parent_level++;
            return make_token(lexer, TOKEN_LEFT_PARENTHESIS);
        case '[': 
            lexer->parent_level++;
            return make_token(lexer, TOKEN_LEFT_BRACKET);
        case '{': 
            lexer->parent_level++;
            return make_token(lexer, TOKEN_LEFT_BRACE);
        case ')': 
            lexer->parent_level--;
            return make_token(lexer, TOKEN_RIGHT_PARENTHESIS);
        case ']': 
            lexer->parent_level--;
            return make_token(lexer, TOKEN_RIGHT_BRACKET);
        case '}': 
            lexer->parent_level--;
            return make_token(lexer, TOKEN_RIGHT_BRACE);
        case ',': 
            return make_token(lexer, TOKEN_COMMA);
        case '.': 
            return make_token(lexer, TOKEN_DOT);
        case ':': 
            return make_token(lexer, TOKEN_COLON);

        // Token de dos caracteres
        case '=': 
            return make_token(lexer, match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '!': 
            return make_token(lexer, match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_ERROR);
        case '<': 
            return make_token(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': 
            return make_token(lexer, match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    }

    return make_token(lexer, TOKEN_ERROR);
}