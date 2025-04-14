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
    RUN_TEST(test_Token_init_EOF);
    RUN_TEST(test_Token_init_unknown);
    RUN_TEST(test_Token_init_error);
    RUN_TEST(test_Token_init_one_char);
    RUN_TEST(test_Token_init_one_or_two_chars);
    RUN_TEST(test_Token_init_identifiers);
    RUN_TEST(test_Token_init_keywords);
    RUN_TEST(test_Token_init_strings);
    RUN_TEST(test_Token_init_numbers);
    RUN_TEST(test_Token_init_comments);
    RUN_TEST(test_Token_free);

    // Tests para lexer.c
    RUN_TEST(test_Lexer_init);
    RUN_TEST(test_Lexer_position_tracking);
    RUN_TEST(test_Lexer_EOF);
    RUN_TEST(test_Lexer_unknown);
    RUN_TEST(test_Lexer_whitespace);
    RUN_TEST(test_Lexer_one_char_token);
    RUN_TEST(test_Lexer_one_or_two_char_token);
    RUN_TEST(test_Lexer_identifier);
    RUN_TEST(test_Lexer_keyword);
    RUN_TEST(test_Lexer_number);
    RUN_TEST(test_Lexer_string);
    RUN_TEST(test_Lexer_comment);

    return UNITY_END();
}