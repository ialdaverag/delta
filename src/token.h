#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    // Palabras clave
    TOKEN_VAR, // var
    TOKEN_CONST, // const
    TOKEN_SI, // si
    TOKEN_SINO, // sino
    TOKEN_SEGUN, // segun
    TOKEN_CASO, // caso
    TOKEN_OTRO, // otro
    TOKEN_MIENTRAS, // mientras
    TOKEN_PARA, // para
    TOKEN_CONTINUAR, // continuar
    TOKEN_ROMPER, // romper
    TOKEN_FUN, // fun
    TOKEN_RET, // retornar
    TOKEN_CLASE, // clase
    TOKEN_Y, // y
    TOKEN_O, // o
    TOKEN_NO, // no
    TOKEN_VERDADERO, // verdadero
    TOKEN_FALSO, // falso
    TOKEN_NULO, // nulo

    // Identificador
    TOKEN_IDENTIFIER, // [a-zA-Z_][a-zA-Z0-9_]*

    // Literales
    TOKEN_INTEGER, // [0-9]+
    TOKEN_FLOAT, // [0-9]+.[0-9]+
    TOKEN_STRING, // ".*" o '.*'

    // Operadores
    TOKEN_EQUAL, // =

    TOKEN_PLUS, // +
    TOKEN_MINUS, // -
    TOKEN_STAR, // *
    TOKEN_SLASH, // /
    TOKEN_PERCENT, // %

    TOKEN_EQUAL_EQUAL, // ==
    TOKEN_BANG_EQUAL, // !=
    TOKEN_LESS, // <
    TOKEN_LESS_EQUAL, // <=
    TOKEN_GREATER, // >
    TOKEN_GREATER_EQUAL, // >=

    // Delimitadores
    TOKEN_LEFT_PARENTHESIS, // (
    TOKEN_LEFT_BRACKET, // [
    TOKEN_LEFT_BRACE, // {
    TOKEN_RIGHT_PARENTHESIS, // )
    TOKEN_RIGHT_BRACKET, // ]
    TOKEN_RIGHT_BRACE, // }
    TOKEN_COMMA, // ,
    TOKEN_DOT, // .
    TOKEN_COLON, // :

    // Comentario
    TOKEN_COMMENT, // #.*

    // Nueva línea
    TOKEN_NEWLINE, // \n

    // Error
    TOKEN_ERROR, // Error de análisis

    // Fin de archivo
    TOKEN_EOF, // \0
} TokenType;

typedef struct Token {
    TokenType type;
    char* lexeme;
    int line;
    int column;
} Token;

void Token_init(Token* token, TokenType type, const char* start, const char* current, int line, int column);
void Token_free(Token* token);
void Token_print(Token token);

#endif // TOKEN_H