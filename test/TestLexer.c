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
    TEST_ASSERT_EQUAL_STRING("ERROR: Caracter inesperado.", token.start);
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

void test_Lexer_operadores_comparacion(void) {
    Lexer lexer;
    Lexer_init(&lexer, "== != < <= > >=");

    TEST_ASSERT_EQUAL(TOKEN_IGUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_DISTINTO, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_MENOR, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_MENOR_IGUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_MAYOR, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_MAYOR_IGUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_FDA, Lexer_next_token(&lexer).type);
}

void test_Lexer_cadena(void) {
    struct {
        const char* input;
        TokenType expected;
    } casos[] = {
        {"\"Hola mundo\"", TOKEN_LITERAL_CADENA},
        {"\"Esto es una prueba\"", TOKEN_LITERAL_CADENA},
        {"\"\"", TOKEN_LITERAL_CADENA},
        {"\"Falta cierre", TOKEN_ERROR},
        {"\"Primera línea \n Segunda línea\"", TOKEN_ERROR}
    };

    for (int i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        Lexer lexer;
        Lexer_init(&lexer, casos[i].input);

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(casos[i].expected, token.type);

        if (token.type != TOKEN_ERROR) {
            TEST_ASSERT_EQUAL(TOKEN_FDA, Lexer_next_token(&lexer).type);
        }
    }
}

void test_Lexer_identificadores(void) {
    Lexer lexer;
    Lexer_init(&lexer, "variable _variable otraVariable");

    TEST_ASSERT_EQUAL(TOKEN_IDENTIFICADOR, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFICADOR, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFICADOR, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_FDA, Lexer_next_token(&lexer).type);
}

void test_lexer_keywords(void) {
    struct {
        const char* input;
        TokenType esperado;
    } casos[] = {
        {"var", TOKEN_VAR},
        {"const", TOKEN_CONST}, 
        {"si", TOKEN_SI},
        {"sino", TOKEN_SINO},
        {"segun", TOKEN_SEGUN},
        {"caso", TOKEN_CASO},
        {"otro", TOKEN_OTRO},
        {"mientras", TOKEN_MIENTRAS},
        {"para", TOKEN_PARA},
        {"continuar", TOKEN_CONTINUAR},
        {"romper", TOKEN_ROMPER},
        {"fun", TOKEN_FUN}
    };

    for (int i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        Lexer lexer;
        Lexer_init(&lexer, casos[i].input);

        Token token = Lexer_next_token(&lexer);

        TEST_ASSERT_EQUAL(casos[i].esperado, token.type);
        TEST_ASSERT_EQUAL(TOKEN_FDA, Lexer_next_token(&lexer).type);
    }
}
