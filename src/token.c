#include "token.h"
#include <stdlib.h>
#include <string.h>

static const char* token_type_to_string(TokenType type) {
    switch (type) {
        // Palabras clave
        case TOKEN_VAR: 
            return "VAR";
        case TOKEN_CONST: 
            return "CONST";
        case TOKEN_SI: 
            return "SI";
        case TOKEN_SINO: 
            return "SINO";
        case TOKEN_SEGUN: 
            return "SEGUN";
        case TOKEN_CASO: 
            return "CASO";
        case TOKEN_OTRO: 
            return "OTRO";
        case TOKEN_MIENTRAS: 
            return "MIENTRAS";
        case TOKEN_PARA: 
            return "PARA";
        case TOKEN_CONTINUAR: 
            return "CONTINUAR";
        case TOKEN_ROMPER: 
            return "ROMPER";
        case TOKEN_FUN: 
            return "FUN";
        case TOKEN_RET: 
            return "RET";
        case TOKEN_CLASE: 
            return "CLASE";
        case TOKEN_Y: 
            return "Y";
        case TOKEN_O: 
            return "O";
        case TOKEN_NO: 
            return "NO";
        case TOKEN_VERDADERO: 
            return "VERDADERO";
        case TOKEN_FALSO: 
            return "FALSO";
        case TOKEN_NULO: 
            return "NULO";

        // Identificador
        case TOKEN_IDENTIFIER: 
            return "IDENTIFIER";

        // Literales
        case TOKEN_INTEGER: 
            return "INTEGER";
        case TOKEN_FLOAT: 
            return "FLOAT";
        case TOKEN_STRING: 
            return "STRING";

        // Operadores
        case TOKEN_EQUAL: 
            return "EQUAL";
        case TOKEN_PLUS: 
            return "PLUS";
        case TOKEN_MINUS: 
            return "MINUS";
        case TOKEN_STAR: 
            return "STAR";
        case TOKEN_SLASH: 
            return "SLASH";
        case TOKEN_PERCENT: 
            return "PERCENT";
        case TOKEN_EQUAL_EQUAL: 
            return "EQUAL_EQUAL";
        case TOKEN_BANG_EQUAL: 
            return "BANG_EQUAL";
        case TOKEN_LESS: 
            return "LESS";
        case TOKEN_LESS_EQUAL: 
            return "LESS_EQUAL";
        case TOKEN_GREATER: 
            return "GREATER";
        case TOKEN_GREATER_EQUAL: 
            return "GREATER_EQUAL";

        // Delimitadores
        case TOKEN_LEFT_PARENTHESIS: 
            return "LEFT_PARENTHESIS";
        case TOKEN_LEFT_BRACKET: 
            return "LEFT_BRACKET";
        case TOKEN_LEFT_BRACE: 
            return "LEFT_BRACE";
        case TOKEN_RIGHT_PARENTHESIS: 
            return "RIGHT_PARENTHESIS";
        case TOKEN_RIGHT_BRACKET: 
            return "RIGHT_BRACKET";
        case TOKEN_RIGHT_BRACE: 
            return "RIGHT_BRACE";
        case TOKEN_COMMA: 
            return "COMMA";
        case TOKEN_DOT: 
            return "DOT";
        case TOKEN_COLON: 
            return "COLON";

        // Comentario
        case TOKEN_COMMENT: 
            return "COMMENT";

        // Nueva línea
        case TOKEN_NEWLINE: 
            return "NEWLINE";
        case TOKEN_NL:
            return "NL";

        // Identación
        case TOKEN_INDENT: 
            return "INDENT";
        case TOKEN_DEDENT:
            return "DEDENT";

        // Errores
        case TOKEN_ERROR: 
            return "ERROR";

        // Fin de archivo
        case TOKEN_EOF: 
            return "EOF";

        // Otros
        default: 
            return "UNKNOWN";
    }
}

void Token_init(Token* token, TokenType type, const char* start, const char* current, int line, int column) {
    token->type = type;
    token->line = line;
    token->column = column;
    token->lexeme = (char*)malloc((current - start) + 1);

    if (token->lexeme != NULL) {
        strncpy(token->lexeme, start, current - start);
        token->lexeme[current - start] = '\0'; 
    }
}

void Token_free(Token* token) {
    if (token->lexeme != NULL) {
        free(token->lexeme);
        token->lexeme = NULL;
    }
}

void Token_print(Token token) {
    printf(
        "Token [L%d:C%d]: %s, %s\n",
        token.line, 
        token.column, 
        token_type_to_string(token.type),
        token.lexeme != NULL ? token.lexeme : "(null)"
    );
}