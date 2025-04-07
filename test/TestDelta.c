#include "unity.h"
#include "TestFile.h"
#include "TestToken.h"
#include "TestLexer.h"

void setUp(void) {
    // Configuración antes de cada prueba
}

void tearDown(void) {
    // Limpieza después de cada prueba
}

int main(void) {
    UNITY_BEGIN();
    
    // Tests para file.c
    RUN_TEST(test_File_init_with_valid_file);
    RUN_TEST(test_File_free);

    // Tests para token.c
    RUN_TEST(test_Token_init);

    // Tests para lexer.c
    RUN_TEST(test_Lexer_init);
    RUN_TEST(test_Lexer_token_fda);
    RUN_TEST(test_Lexer_ignore_whitespace);
    RUN_TEST(test_Lexer_unexpected_character);
    RUN_TEST(test_Lexer_token_delimitadores);
    RUN_TEST(test_Lexer_operador_asignacion);
    RUN_TEST(test_Lexer_operadores_aritmeticos);
    RUN_TEST(test_Lexer_operadores_comparacion);
    RUN_TEST(test_Lexer_cadena);
    RUN_TEST(test_Lexer_identificadores);
    RUN_TEST(test_Lexer_numeros_enteros);
    RUN_TEST(test_Lexer_numeros_decimales);
    RUN_TEST(test_lexer_keywords);
    RUN_TEST(test_Lexer_comments);

    return UNITY_END();
}