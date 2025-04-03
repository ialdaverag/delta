#include "TestLexer.h"

void test_Lexer_init() {
    const char* source = "var x = 10";
    
    Lexer lexer;
    Lexer_init(&lexer, source);

    TEST_ASSERT_EQUAL_STRING(source, lexer.start);
    TEST_ASSERT_EQUAL_STRING(source, lexer.current);
    TEST_ASSERT_EQUAL(1, lexer.line);
}

void test_Lexer_token_fda(void) {
    Lexer lexer;
    Lexer_init(&lexer, "");

    Token token = Lexer_next_token(&lexer);
    TEST_ASSERT_EQUAL_INT(TOKEN_FDA, token.type);
}