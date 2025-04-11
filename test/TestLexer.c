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
    TEST_ASSERT_EQUAL(TOKEN_UNKNOWN, token.type);
}

void test_Lexer_token_fda(void) {
    Lexer lexer;
    Lexer_init(&lexer, "");

    Token token = Lexer_next_token(&lexer);
    TEST_ASSERT_EQUAL_INT(TOKEN_EOF, token.type);
}

void test_Lexer_ignore_whitespace(void) {
    Lexer lexer;
    Lexer_init(&lexer, "   \t\n  ");

    Token token = Lexer_next_token(&lexer);
    TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
}

void test_Lexer_token_delimitadores(void) {
    Lexer lexer;
    Lexer_init(&lexer, "(){}[],.:");

    TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_PARENTHESIS, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_PARENTHESIS, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_BRACE, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_BRACE, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_LEFT_BRACKET, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_BRACKET, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_DOT, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL_INT(TOKEN_COLON, Lexer_next_token(&lexer).type);
}

void test_Lexer_operador_asignacion(void) {
    Lexer lexer;
    Lexer_init(&lexer, "=");

    TEST_ASSERT_EQUAL(TOKEN_EQUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
}

void test_Lexer_operadores_aritmeticos(void) {
    Lexer lexer;
    Lexer_init(&lexer, "+-*/%");

    TEST_ASSERT_EQUAL(TOKEN_PLUS, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_MINUS, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_STAR, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_SLASH, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_PERCENT, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
}

void test_Lexer_operadores_comparacion(void) {
    Lexer lexer;
    Lexer_init(&lexer, "== != < <= > >=");

    TEST_ASSERT_EQUAL(TOKEN_EQUAL_EQUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_BANG_EQUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_LESS, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_LESS_EQUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_GREATER, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_GREATER_EQUAL, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
}

void test_Lexer_cadena(void) {
    struct {
        const char* input;
        TokenType expected;
    } casos[] = {
        // Comillas dobles
        {"\"Hola mundo\"", TOKEN_STRING},
        {"\"Esto es una prueba\"", TOKEN_STRING},
        {"\"\"", TOKEN_STRING},
        {"\"Falta cierre", TOKEN_ERROR},
        {"\"Primera línea \n Segunda línea\"", TOKEN_ERROR},

        // Comillas simples
        {"'Hola mundo'", TOKEN_STRING},
        {"'Esto es una prueba'", TOKEN_STRING},
        {"''", TOKEN_STRING},
        {"'Falta cierre", TOKEN_ERROR},
        {"'Primera línea \n Segunda línea'", TOKEN_ERROR}
    };

    for (int i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        Lexer lexer;
        Lexer_init(&lexer, casos[i].input);

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(casos[i].expected, token.type);

        if (token.type != TOKEN_ERROR) {
            TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
        }
    }
}


void test_Lexer_numeros_enteros(void) {
    const char* casos[] = {
        "10",
        "123",
        "4567",
        "0",
        "987654",
    };

    for (int i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        Lexer lexer;
        Lexer_init(&lexer, casos[i]);

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_INTEGER, token.type);
        TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
    }
}

void test_Lexer_numeros_decimales(void) {
    const char* casos[] = {
        "10.5",
        "123.45",
        "0.99",
        "0.0",
    };

    for (int i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        Lexer lexer;
        Lexer_init(&lexer, casos[i]);

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_FLOAT, token.type);
        TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
    }
}

void test_Lexer_identificadores(void) {
    Lexer lexer;
    Lexer_init(&lexer, "variable _variable otraVariable conio");

    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, Lexer_next_token(&lexer).type);
    TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
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
        {"fun", TOKEN_FUN},
        {"ret", TOKEN_RET},
        {"clase", TOKEN_CLASE},
        {"y", TOKEN_Y},
        {"o", TOKEN_O},
        {"no", TOKEN_NO},
        {"verdadero", TOKEN_VERDADERO},
        {"falso", TOKEN_FALSO},
        {"nulo", TOKEN_NULO}
    };

    for (int i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        Lexer lexer;
        Lexer_init(&lexer, casos[i].input);

        Token token = Lexer_next_token(&lexer);

        TEST_ASSERT_EQUAL(casos[i].esperado, token.type);
        TEST_ASSERT_EQUAL(TOKEN_EOF, Lexer_next_token(&lexer).type);
    }
}