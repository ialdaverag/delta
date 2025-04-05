#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    // Palabras clave
    TOKEN_VAR, // var
    TOKEN_CONST, // const
    TOKEN_SI, // si
    TOKEN_SINO, // sino
    TOKEN_SINO_SI, // sino si
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
    TOKEN_ES, // es
    TOKEN_EN, // en
    TOKEN_IMPORTAR, // importar
    TOKEN_DESDE, // desde
    TOKEN_COMO, // como

    // Identificador
    TOKEN_IDENTIFICADOR, // [a-zA-Z_][a-zA-Z0-9_]*

    // Literales
    TOKEN_LITERAL_ENTERO, // [0-9]+
    TOKEN_LITERAL_REAL, // [0-9]+.[0-9]+
    TOKEN_LITERAL_CADENA, // ".*" o '.*'

    // Operadores
    TOKEN_ASIGNACION, // =

    TOKEN_MAS, // +
    TOKEN_MENOS, // -
    TOKEN_ASTERISCO, // *
    TOKEN_DIAGONAL, // /
    TOKEN_PORCENTAJE, // %

    TOKEN_IGUAL, // ==
    TOKEN_DISTINTO, // !=
    TOKEN_MAYOR, // >
    TOKEN_MAYOR_IGUAL, // >=
    TOKEN_MENOR, // <
    TOKEN_MENOR_IGUAL, // <=

    // Delimitadores
    TOKEN_PARENTESIS_IZQUIERDO, // (
    TOKEN_PARENTESIS_DERECHO, // )
    TOKEN_CORCHETE_IZQUIERDO, // [
    TOKEN_CORCHETE_DERECHO, // ]
    TOKEN_LLAVE_IZQUIERDA, // {
    TOKEN_LLAVE_DERECHA, // }
    TOKEN_PUNTO, // .
    TOKEN_COMA, // ,
    TOKEN_DOS_PUNTOS, // :
    TOKEN_COMILLA, // "

    // Comentarios
    TOKEN_COMENTARIO, // #.*

    // Fin de archivo
    TOKEN_FDA, // \0

    // Errores
    TOKEN_ERROR, // Error de análisis
} TokenType;

typedef struct Token {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

Token Token_init(Token* token, TokenType type, const char* start, const char* current, int line);

#endif // TOKEN_H