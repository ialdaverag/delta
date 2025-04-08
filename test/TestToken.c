#include "TestToken.h"

void test_Token_init() {
    struct {
        TokenType type;
        const char* lexeme;
    } keywords[] = {
        {TOKEN_VAR, "var"},
        {TOKEN_CONST, "const"},
        {TOKEN_SI, "si"},
        {TOKEN_SINO, "sino"},
        {TOKEN_SINO_SI, "sino si"},
        {TOKEN_SEGUN, "segun"},
        {TOKEN_CASO, "caso"},
        {TOKEN_OTRO, "otro"},
        {TOKEN_MIENTRAS, "mientras"},
        {TOKEN_PARA, "para"},
        {TOKEN_CONTINUAR, "continuar"},
        {TOKEN_ROMPER, "romper"},
        {TOKEN_FUN, "funcion"},
        {TOKEN_RET, "retornar"},
        {TOKEN_CLASE, "clase"},
        {TOKEN_Y, "y"},
        {TOKEN_O, "o"},
        {TOKEN_NO, "no"},
        {TOKEN_VERDADERO, "verdadero"},
        {TOKEN_FALSO, "falso"},
        {TOKEN_NULO, "nulo"},

        {TOKEN_IDENTIFICADOR, "identificador"},
        {TOKEN_LITERAL_ENTERO, "123"},
        {TOKEN_LITERAL_REAL, "123.45"},
        {TOKEN_LITERAL_CADENA, "\"cadena\""},

        {TOKEN_ASIGNACION, "="},

        {TOKEN_MAS, "+"},
        {TOKEN_MENOS, "-"},
        {TOKEN_ASTERISCO, "*"},
        {TOKEN_DIAGONAL, "/"},
        {TOKEN_PORCENTAJE, "%"},

        {TOKEN_IGUAL, "=="},
        {TOKEN_DISTINTO, "!="},
        {TOKEN_MAYOR, ">"},
        {TOKEN_MAYOR_IGUAL, ">="},
        {TOKEN_MENOR, "<"},
        {TOKEN_MENOR_IGUAL, "<="},

        {TOKEN_PARENTESIS_IZQUIERDO, "("},
        {TOKEN_PARENTESIS_DERECHO, ")"},
        {TOKEN_CORCHETE_IZQUIERDO, "["},
        {TOKEN_CORCHETE_DERECHO, "]"},
        {TOKEN_LLAVE_IZQUIERDA, "{"},
        {TOKEN_LLAVE_DERECHA, "}"},
        {TOKEN_PUNTO, "."},
        {TOKEN_COMA, ","},
        {TOKEN_DOS_PUNTOS, ":"},

        {TOKEN_COMENTARIO, "# comentario"},

        {TOKEN_FDA, "\0"}
    };

    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        TokenType type = keywords[i].type;
        const char* start = keywords[i].lexeme;
        const char* current = start + strlen(start);
        int line = 1;
        int column = 1 + (int)(current - start);

        Token token;
        Token_init(&token, type, start, current, line, column);

        TEST_ASSERT_EQUAL(type, token.type);
        TEST_ASSERT_EQUAL_STRING(start, token.start);
        TEST_ASSERT_EQUAL(strlen(start), token.length);
        TEST_ASSERT_EQUAL(1, token.line);
    }
}
