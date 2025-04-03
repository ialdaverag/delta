#include "TestLexer.h"

void test_Lexer_init() {
    const char* source = "var x = 10";
    
    Lexer lexer;
    Lexer_init(&lexer, source);

    TEST_ASSERT_EQUAL_STRING(source, lexer.start);
    TEST_ASSERT_EQUAL_STRING(source, lexer.current);
    TEST_ASSERT_EQUAL(1, lexer.line);
}

void test_Lexer_unexpected_character(void) {
    Lexer lexer;
    Lexer_init(&lexer, "@");

    Token token = Lexer_next_token(&lexer);
    TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
    TEST_ASSERT_EQUAL_STRING("Caracter inesperado.", token.start);
}

void test_Lexer_token_fda(void) {
    Lexer lexer;
    Lexer_init(&lexer, "");

    Token token = Lexer_next_token(&lexer);
    TEST_ASSERT_EQUAL_INT(TOKEN_FDA, token.type);
}

void test_Lexer_ignore_whitespace(void) {
    Lexer lexer;
    Lexer_init(&lexer, "   \t\n  ");

    Token token = Lexer_next_token(&lexer);
    TEST_ASSERT_EQUAL(TOKEN_FDA, token.type);
}

void test_Lexer_token_delimitadores(void) {
    Lexer lexer;
    Lexer_init(&lexer, "(){}[],.:");

    TEST_ASSERT_EQUAL_INT(TOKEN_PARENTESIS_IZQUIERDO, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_PARENTESIS_DERECHO, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_LLAVE_IZQUIERDA, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_LLAVE_DERECHA, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_CORCHETE_IZQUIERDO, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_CORCHETE_DERECHO, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_COMA, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_PUNTO, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_DOS_PUNTOS, Lexer_next_token(&lexer).type);
}

void test_Lexer_operador_asignacion(void) {
    Lexer lexer;
    Lexer_init(&lexer, "=");

    TEST_ASSERT_EQUAL(TOKEN_ASIGNACION, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_FDA, Lexer_next_token(&lexer).type);
}

void test_Lexer_operadores_aritmeticos(void) {
    Lexer lexer;
    Lexer_init(&lexer, "+-*/%");

    TEST_ASSERT_EQUAL(TOKEN_MAS, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_MENOS, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_ASTERISCO, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_DIAGONAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_PORCENTAJE, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_FDA, Lexer_next_token(&lexer).type);
}