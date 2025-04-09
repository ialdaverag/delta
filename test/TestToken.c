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

        {TOKEN_IDENTIFIER, "identificador"},
        {TOKEN_INTEGER, "123"},
        {TOKEN_FLOAT, "123.45"},
        {TOKEN_STRING, "\"cadena\""},

        {TOKEN_EQUAL, "="},

        {TOKEN_PLUS, "+"},
        {TOKEN_MINUS, "-"},
        {TOKEN_STAR, "*"},
        {TOKEN_SLASH, "/"},
        {TOKEN_PERCENT, "%"},

        {TOKEN_EQUAL_EQUAL, "=="},
        {TOKEN_BANG_EQUAL, "!="},
        {TOKEN_GREATER, ">"},
        {TOKEN_GREATER_EQUAL, ">="},
        {TOKEN_LESS, "<"},
        {TOKEN_LESS_EQUAL, "<="},

        {TOKEN_LEFT_PARENTHESIS, "("},
        {TOKEN_RIGHT_PARENTHESIS, ")"},
        {TOKEN_LEFT_BRACKET, "["},
        {TOKEN_RIGHT_BRACKET, "]"},
        {TOKEN_LEFT_BRACE, "{"},
        {TOKEN_RIGHT_BRACE, "}"},
        {TOKEN_DOT, "."},
        {TOKEN_COMMA, ","},
        {TOKEN_COLON, ":"},

        {TOKEN_EOF, "\0"}
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
