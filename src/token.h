#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    // Fin de archivo
    TOKEN_EOF, // \0

    // Nueva línea
    TOKEN_NEWLINE, // \n
    TOKEN_NL, // \n

    // Identación
    TOKEN_INDENT,
    TOKEN_DEDENT,
    TOKEN_NULL,

    // Comentario
    TOKEN_COMMENT, // #.*

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

    // Tokens de un caracter
    TOKEN_PLUS, // +
    TOKEN_MINUS, // -
    TOKEN_STAR, // *
    TOKEN_SLASH, // /
    TOKEN_PERCENT, // %
    TOKEN_BANG, // !
    TOKEN_EQUAL, // =
    TOKEN_LESS, // <
    TOKEN_GREATER, // >
    TOKEN_LEFT_PARENTHESIS, // (
    TOKEN_LEFT_BRACKET, // [
    TOKEN_LEFT_BRACE, // {
    TOKEN_RIGHT_PARENTHESIS, // )
    TOKEN_RIGHT_BRACKET, // ]
    TOKEN_RIGHT_BRACE, // }
    TOKEN_COMMA, // ,
    TOKEN_DOT, // .
    TOKEN_COLON, // :
    TOKEN_SEMICOLON, // ;

    // Tokens de dos caracteres
    TOKEN_EQUAL_EQUAL, // ==
    TOKEN_BANG_EQUAL, // !=
    TOKEN_LESS_EQUAL, // <=
    TOKEN_GREATER_EQUAL, // >=

    // Tokens de tres caracteres
    TOKEN_ELLIPSIS, // ...

    // Desconocido
    TOKEN_UNKNOWN,

    // Error
    TOKEN_ERROR, // Error de análisis
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