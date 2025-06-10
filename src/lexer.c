#include "lexer.h"

void Lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;

    lexer->token_line = 1;
    lexer->token_column = 1;

    lexer->saw_token = false;

    lexer->indent_stack[0] = 0;
    lexer->indent_top = 0;
    lexer->pending = 0;
    lexer->atbol = true;

    lexer->parent_top = 0;
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

static void mark_start(Lexer* lexer) {
    lexer->start = lexer->current;
    lexer->token_line = lexer->line;
    lexer->token_column = lexer->column;
}

static void skip_whitespace(Lexer* lexer) {
    while (peek(lexer) == ' ' || peek(lexer) == '\t') {
        advance(lexer);
    }
}

static void push_parent(Lexer* lexer, char c) {
    if (lexer->parent_top >= MAX_PARENT_STACK) {
        fprintf(stderr, "ERROR: Demasiada profundidad de paréntesis.\n");
        exit(1);
    }

    lexer->parent_stack[lexer->parent_top] = c;
    lexer->parent_line_stack[lexer->parent_top] = lexer->line;
    lexer->parent_column_stack[lexer->parent_top] = lexer->column;
    lexer->parent_top++;
}

static bool pop_parent(Lexer* lexer, char closing) {
    if (lexer->parent_top == 0) {
        return false;
    }

    char opening = lexer->parent_stack[lexer->parent_top - 1];

    bool matches = (opening == '(' && closing == ')') ||
                   (opening == '[' && closing == ']') ||
                   (opening == '{' && closing == '}');

    if (matches) {
        lexer->parent_top--;

        return true;
    }

    return false;
}

static bool is_eof(char c) {
    return c == '\0';
}

static bool is_newline(char c) {
    return c == '\n' || c == '\r';  // Considera ambos como newline
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

static bool is_delimiter(char c) {
    return c == '(' || c == ')' || 
           c == '[' || c == ']' || 
           c == '{' || c == '}';
}

static bool is_punctuation(char c) {
    return c == '+' || 
           c == '-' || 
           c == '*' || 
           c == '/' || 
           c == '%' || 
           c == ',' || 
           c == '.' || 
           c == ':' || 
           c == ';' || 
           c == '=' || 
           c == '!' || 
           c == '<' || 
           c == '>';
}

static bool is_keyword(
    Lexer* lexer, 
    int length, 
    int offset, 
    const char* keyword, 
    int keyword_length) {
    return (lexer->current - lexer->start == length) && 
           (memcmp(lexer->start + offset, keyword, keyword_length) == 0);
}

static Token make_token(Lexer* lexer, TokenType type) {
    // Marcar token como significativo
    if (type != TOKEN_COMMENT && type != TOKEN_NEWLINE && 
        type != TOKEN_NL && type != TOKEN_EOF &&
        type != TOKEN_NULL && type != TOKEN_ERROR) {
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

static Token get_pending_indentation_token(Lexer* lexer) {
    if (lexer->pending > 0) {
        lexer->pending--;
        return make_token(lexer, TOKEN_INDENT);
    } else {
        lexer->pending++;
        return make_token(lexer, TOKEN_DEDENT);
    }
}

static Token indent(Lexer* lexer) {
    if (lexer->atbol && lexer->parent_top == 0) {
        mark_start(lexer);
        
        int col = 0;
        bool has_tabs = false;
        bool has_spaces = false;
        
        while (peek(lexer) == ' ' || peek(lexer) == '\t') {
            if (peek(lexer) == ' ') {
                col++;
                has_spaces = true;
            } else {
                col += TAB_SIZE;
                has_tabs = true;
            }

            if (has_tabs && has_spaces) {
                lexer->current = lexer->start;

                return error_token(lexer, "ERROR: Mezcla de espacios y tabulaciones en la indentacion.");
            }

            advance(lexer);
        }
        
        lexer->atbol = false;
        
        if (peek(lexer) == '\n' || peek(lexer) == '\r' || peek(lexer) == '#' || peek(lexer) == '\0') {
            return make_token(lexer, TOKEN_NULL);
        } 

        if (col > lexer->indent_stack[lexer->indent_top]) {
            if (lexer->indent_top + 1 >= MAX_INDENT_STACK) {
                lexer->current = lexer->start;

                return error_token(lexer, "ERROR: Excedido el nivel maximo de indentacion.");
            }

            lexer->indent_stack[++lexer->indent_top] = col;
            lexer->pending = 1;
        } else if (col < lexer->indent_stack[lexer->indent_top]) {
            while (lexer->indent_top > 0 && col < lexer->indent_stack[lexer->indent_top]) {
                lexer->indent_top--;
                lexer->pending--;
            }
            
            if (col != lexer->indent_stack[lexer->indent_top]) {
                lexer->pending = 0;
                lexer->current = lexer->start;

                return error_token(lexer, "ERROR: Indentacion no valida.");
            }
        }
    }

    return make_token(lexer, TOKEN_NULL);
}

static Token eof(Lexer* lexer) {
    if (lexer->parent_top > 0) {
        return error_token(lexer, "ERROR: Parentesis no cerrado al final del archivo.");
    }

    return make_token(lexer, TOKEN_EOF);
}

static Token newline(Lexer* lexer) {
    Token token;
    
    if (lexer->parent_top > 0 || !lexer->saw_token) {
        token = make_token(lexer, TOKEN_NL);
    } else {
        token = make_token(lexer, TOKEN_NEWLINE);
    }

    lexer->saw_token = false;
    lexer->atbol = true;
    
    return token;
}

static Token comment(Lexer* lexer) {
    while (!is_at_end(lexer) && !is_newline(peek(lexer))) {
        advance(lexer);
    }

    return make_token(lexer, TOKEN_COMMENT);
}

static Token identifier_or_keyword(Lexer* lexer) {
    // Avanzar sobre caracteres alfabéticos y numéricos
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer))) {
        advance(lexer);
    }

    // Determinar el tipo de identificador o palabra clave
    int length = lexer->current - lexer->start;

    switch (lexer->start[0]) {
        case 'c':
            if (is_keyword(lexer, 4, 1, "aso", 3)) {
                return make_token(lexer, TOKEN_CASO);
            }

            if (is_keyword(lexer, 5, 1, "lase", 4)) {
                return make_token(lexer, TOKEN_CLASE);
            }

            if (is_keyword(lexer, 5, 1, "onst", 4)) {
                return make_token(lexer, TOKEN_CONST);
            }

            if (is_keyword(lexer, 9, 1, "ontinuar", 8)) {
                return make_token(lexer, TOKEN_CONTINUAR);
            }

            break;
        case 'f':
            if (is_keyword(lexer, 3, 1, "un", 2)) {
                return make_token(lexer, TOKEN_FUN);
            }

            if (is_keyword(lexer, 5, 1, "also", 4)) {
                return make_token(lexer, TOKEN_FALSO);
            }

            break;
        case 'm':
            if (is_keyword(lexer, 8, 1, "ientras", 7)) {
                return make_token(lexer, TOKEN_MIENTRAS);
            }

            break;
        case 'n':
            if (is_keyword(lexer, 2, 1, "o", 1)) {
                return make_token(lexer, TOKEN_NO);
            }

            if (is_keyword(lexer, 4, 1, "ulo", 3)) {
                return make_token(lexer, TOKEN_NULO);
            }

            break;
        case 'o':
            if (is_keyword(lexer, 4, 1, "tro", 3)) {
                return make_token(lexer, TOKEN_OTRO);
            }

            if (length == 1) {
                return make_token(lexer, TOKEN_O);
            }

            break;
        case 'p':
            if (is_keyword(lexer, 4, 1, "ara", 3)) {
                return make_token(lexer, TOKEN_PARA);
            }

            break;
        case 'r':
            if (is_keyword(lexer, 3, 1, "et", 2)) {
                return make_token(lexer, TOKEN_RET);
            }

            if (is_keyword(lexer, 6, 1, "omper", 5)) {
                return make_token(lexer, TOKEN_ROMPER);
            }

            break;
        case 's':
            if (is_keyword(lexer, 2, 1, "i", 1)) {
                return make_token(lexer, TOKEN_SI);
            }

            if (is_keyword(lexer, 4, 1, "ino", 3)) {
                return make_token(lexer, TOKEN_SINO);
            }

            if (is_keyword(lexer, 5, 1, "egun", 4)) {
                return make_token(lexer, TOKEN_SEGUN);
            }

            break;
        case 'v':
            if (is_keyword(lexer, 3, 1, "ar", 2)) {
                return make_token(lexer, TOKEN_VAR);
            }

            if (is_keyword(lexer, 9, 1, "erdadero", 8)) {
                return make_token(lexer, TOKEN_VERDADERO);
            }

            break;
        case 'y':
            if (length == 1) {
                return make_token(lexer, TOKEN_Y);
            }

            break;
    }

    // Si no es una palabra clave, es un identificador genérico
    return make_token(lexer, TOKEN_IDENTIFIER);
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

        if (peek(lexer) == '\\') {
            advance(lexer); // Saltar el carácter '\'
            if (!is_at_end(lexer)) {
                advance(lexer); // Saltar el carácter siguiente después de '\'
            }
        } else {
            advance(lexer); // Avanzar normalmente
        }
    }

    if (is_at_end(lexer)) {
        return error_token(lexer, "ERROR: Cadena no terminada.");
    }

    advance(lexer); // Consumir la comilla de cierre

    return make_token(lexer, TOKEN_STRING);
}

static Token delimiter(Lexer* lexer, char c) {
    if (c == '(' || c == '[' || c == '{') {
        push_parent(lexer, c);

        return make_token(lexer, 
            c == '(' ? TOKEN_LEFT_PARENTHESIS : 
            c == '[' ? TOKEN_LEFT_BRACKET : 
                       TOKEN_LEFT_BRACE);
    } else if (c == ')' || c == ']' || c == '}') {
        if (!pop_parent(lexer, c)) {
            return error_token(lexer, "ERROR: Parentesis de cierre inesperado o no coincide.");
        }

        return make_token(lexer, 
            c == ')' ? TOKEN_RIGHT_PARENTHESIS : 
            c == ']' ? TOKEN_RIGHT_BRACKET : 
                       TOKEN_RIGHT_BRACE);
    }

    return make_token(lexer, TOKEN_UNKNOWN);
}

static Token one_char(Lexer* lexer, char c1) {
    switch (c1) {
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
            return make_token(lexer, TOKEN_LEFT_PARENTHESIS);
        case ')': 
            return make_token(lexer, TOKEN_RIGHT_PARENTHESIS);
        case '[': 
            return make_token(lexer, TOKEN_LEFT_BRACKET);
        case ']': 
            return make_token(lexer, TOKEN_RIGHT_BRACKET);
        case '{': 
            return make_token(lexer, TOKEN_LEFT_BRACE);
        case '}': 
            return make_token(lexer, TOKEN_RIGHT_BRACE);
        case ',': 
            return make_token(lexer, TOKEN_COMMA);
        case '.': 
            return make_token(lexer, TOKEN_DOT);
        case ':': 
            return make_token(lexer, TOKEN_COLON);
        case ';': 
            return make_token(lexer, TOKEN_SEMICOLON);
        case '=': 
            return make_token(lexer, TOKEN_EQUAL);
        case '!': 
            return make_token(lexer, TOKEN_BANG);
        case '<': 
            return make_token(lexer, TOKEN_LESS);
        case '>': 
            return make_token(lexer, TOKEN_GREATER);
    }

    return make_token(lexer, TOKEN_UNKNOWN);
}

static Token two_chars(Lexer* lexer, char c1, char c2) {
    switch (c1) {
        case '=': 
            switch (c2) {
                case '=': 
                    advance(lexer);
                    return make_token(lexer, TOKEN_EQUAL_EQUAL);
            }
            break;
        case '!': 
            switch (c2) {
                case '=': 
                    advance(lexer);
                    return make_token(lexer, TOKEN_BANG_EQUAL);
            }
            break;
        case '<': 
            switch (c2) {
                case '=': 
                    advance(lexer);
                    return make_token(lexer, TOKEN_LESS_EQUAL);
            }
            break;
        case '>': 
            switch (c2) {
                case '=': 
                    advance(lexer);
                    return make_token(lexer, TOKEN_GREATER_EQUAL);
            }
            break;
    }

    return make_token(lexer, TOKEN_UNKNOWN);
}

static Token three_chars(Lexer* lexer, char c1, char c2, char c3) {
    switch (c1) {
        case '.':
            switch (c2) {
                case '.':
                    switch (c3) {
                        case '.':
                            advance(lexer);
                            advance(lexer);
                            return make_token(lexer, TOKEN_ELLIPSIS);
                    }
                    break;
            }
            break;
    }

    return make_token(lexer, TOKEN_UNKNOWN);
}

Token Lexer_next_token(Lexer* lexer) {
    // Manejar indentación y tokens pendientes
    Token indent_token = indent(lexer);
    if (indent_token.type != TOKEN_NULL) {
        return indent_token;
    }

    // Verificar si hay tokens pendientes de indentación
    if (lexer->pending != 0) {
        return get_pending_indentation_token(lexer);
    }

    // Si estamos al final del archivo y aún hay indentación pendiente
    if (is_at_end(lexer) && lexer->indent_top > 0) {
        lexer->indent_top--;
        lexer->pending = -1;
        
        return get_pending_indentation_token(lexer);
    }

    // Esquivar espacios en blanco (sin procesar indentación)
    skip_whitespace(lexer);

    // Marcar el inicio del token
    mark_start(lexer);

    // Avanzar al siguiente carácter
    char c = advance(lexer);
    
    // EOF
    if (is_eof(c)) {
        return eof(lexer);
    }

    // Nueva línea
    if (is_newline(c)) {
        return newline(lexer);
    }

    // Comentario
    if (is_hash(c)) {
        return comment(lexer);
    }

    // Identificador o palabra clave
    if (is_alpha(c)) {
        return identifier_or_keyword(lexer);
    }

    // Numero
    if (is_digit(c)) {
        return number(lexer);
    }

    // Cadena
    if (is_quote(c)) {
        return string(lexer);
    }

    // Delimitadores
    if (is_delimiter(c)) {
        return delimiter(lexer, c);
    }

    // Verificar tokens de tres caracteres
    if (!is_at_end(lexer)) {
        char c2 = peek(lexer);
        char c3 = peek_next(lexer);

        Token triple_token = three_chars(lexer, c, c2, c3);
        if (triple_token.type != TOKEN_UNKNOWN) {
            return triple_token;
        }
    }

    // Verificar tokens de dos caracteres
    if (!is_at_end(lexer)) {
        char c2 = peek(lexer);
        Token double_token = two_chars(lexer, c, c2);
        
        if (double_token.type != TOKEN_UNKNOWN) {
            return double_token;
        }
    }

    // Tokens de un solo carácter
    Token single_token = one_char(lexer, c);
    if (single_token.type != TOKEN_UNKNOWN) {
        return single_token;
    }

    return error_token(lexer, "ERROR: Caracter inesperado.");
}