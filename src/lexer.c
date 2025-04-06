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

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
            c == '_';
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool match(Lexer* lexer, char expected) {
    if (is_at_end(lexer)) 
        return false;

    if (*lexer->current != expected) 
        return false;

    lexer->current++;
    
    return true;
}

static bool is_string(Lexer* lexer) {
    while (peek(lexer) != '"' && !is_at_end(lexer)) {
        if (peek(lexer) == '\n') {
            advance(lexer);  // Consume el '\n' para no bloquear el lexer

            return false;
        }

        advance(lexer);
    }

    if (is_at_end(lexer)) {
        return false;
    }

    advance(lexer);  // Consume la comilla de cierre

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

static TokenType check_keyword(Lexer* lexer, int start, int length, const char* rest, TokenType type) {
    if (lexer->current - lexer->start == start + length && memcmp(lexer->start + start, rest, length) == 0)
        return type;

    return TOKEN_IDENTIFICADOR;
}

static TokenType identifier_type(Lexer* lexer) {
    switch (lexer->start[0]) {
        case 'a':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 's':
                        if (lexer->current - lexer->start > 2) {
                            switch (lexer->start[2]) {
                                case 'e': 
                                    return check_keyword(lexer, 3, 5, "gurar", TOKEN_ASEGURAR);
                                case 'i': 
                                    return check_keyword(lexer, 3, 2, "nc", TOKEN_ASINC);
                            }
                        }
                        break;
                    case 'g':
                        return check_keyword(lexer, 2, 4, "uard", TOKEN_AGUARD);
                    case 't': 
                        return check_keyword(lexer, 2, 5, "rapar", TOKEN_ATRAPAR);
                }
            }
            break;
        case 'c':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a':
                        return check_keyword(lexer, 2, 2, "so", TOKEN_CASO);
                    case 'o':
                        if (lexer->current - lexer->start > 2) {
                            switch (lexer->start[2]) {
                                case 'n':
                                    if (lexer->current - lexer->start == 3) {
                                        return TOKEN_CON;
                                    }
                                    if (lexer->current - lexer->start > 3) {
                                        switch (lexer->start[3]) {
                                            case 's':
                                                return check_keyword(lexer, 4, 1, "t", TOKEN_CONST);
                                            case 't':
                                                return check_keyword(lexer, 4, 5, "inuar", TOKEN_CONTINUAR);
                                        }
                                    }
                                    break;
                                case 'm':
                                    return check_keyword(lexer, 3, 1, "o", TOKEN_COMO);
                            }
                        }
                        break;
                    case 'l':
                        return check_keyword(lexer, 2, 3, "ase", TOKEN_CLASE);
                }
            }
            break;
        case 'd':
            return check_keyword(lexer, 1, 4, "esde", TOKEN_DESDE);
        case 'e':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'n':
                        return check_keyword(lexer, 2, 0, "", TOKEN_EN);
                    case 's':
                        return check_keyword(lexer, 2, 0, "", TOKEN_ES);
                }
            }
            break;
        case 'f':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a': 
                        return check_keyword(lexer, 2, 3, "lso", TOKEN_FALSO);
                    case 'i':
                        return check_keyword(lexer, 2, 8, "nalmente", TOKEN_FINALMENTE);
                    case 'u': 
                        return check_keyword(lexer, 2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'g':
            return check_keyword(lexer, 1, 2, "en", TOKEN_GEN);
        case 'i':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'm':
                        return check_keyword(lexer, 2, 6, "portar", TOKEN_IMPORTAR);
                    case 'n':
                        return check_keyword(lexer, 2, 6, "tentar", TOKEN_INTENTAR);
                }
            }
            break;
        case 'l':
            return check_keyword(lexer, 1, 5, "anzar", TOKEN_LANZAR);
        case 'm':
            return check_keyword(lexer, 1, 7, "ientras", TOKEN_MIENTRAS);
        case 'n':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'o': 
                        return check_keyword(lexer, 2, 0, "", TOKEN_NO);
                    case 'u': 
                        return check_keyword(lexer, 2, 2, "lo", TOKEN_NULO);
                }
            }
            break;
        case 'o':
            if (lexer->current - lexer->start > 1) 
                return check_keyword(lexer, 1, 3, "tro", TOKEN_OTRO);

            return TOKEN_O;
        case 'p':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a':
                        if (lexer->current - lexer->start > 2) {
                            switch (lexer->start[2]) {
                                case 'r':
                                    return check_keyword(lexer, 3, 1, "a", TOKEN_PARA);
                                case 's':
                                    return check_keyword(lexer, 3, 2, "ar", TOKEN_PASAR);
                            }
                        }
                        break;
                }
            }
            break;
        case 'r':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'e': 
                        return check_keyword(lexer, 2, 1, "t", TOKEN_RET);
                    case 'o': 
                        return check_keyword(lexer, 2, 4, "mper", TOKEN_ROMPER);
                }
            }
            break;
        case 's':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'e':
                        return check_keyword(lexer, 1, 4, "egun", TOKEN_SEGUN);
                    case 'i':
                        if (lexer->current - lexer->start == 2)
                            return TOKEN_SI;
                            
                        return check_keyword(lexer, 2, 2, "no", TOKEN_SINO);
                }
            }
            break;
        case 'v':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a': 
                        return check_keyword(lexer, 2, 1, "r", TOKEN_VAR);
                    case 'e': 
                        return check_keyword(lexer, 2, 7, "rdadero", TOKEN_VERDADERO);
                }
            }
            break;
        case 'y':
            return check_keyword(lexer, 1, 0, "", TOKEN_Y);
    }
    
    return TOKEN_IDENTIFICADOR;
}

static Token identifier(Lexer* lexer) {
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer)))
        advance(lexer);
    
    return make_token(lexer, identifier_type(lexer));
}

Token Lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);

    lexer->start = lexer->current;

    if (is_at_end(lexer)) 
        return make_token(lexer, TOKEN_FDA);

    char c = advance(lexer);

    if (is_alpha(c)) 
        return identifier(lexer);

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

        // Cadena
        case '"':
            if (!is_string(lexer))
                return error_token(lexer, "ERROR: Cadena no terminada.");
            
            return make_token(lexer, TOKEN_LITERAL_CADENA);
    }

    return error_token(lexer, "ERROR: Caracter inesperado.");
}