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

    return UNITY_END();
}