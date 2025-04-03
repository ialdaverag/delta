#ifndef TEST_LEXER_H
#define TEST_LEXER_H

#include "unity.h"
#include "../src/lexer.h"

void test_Lexer_init(void);
void test_Lexer_token_fda(void);
void test_Lexer_ignore_whitespace(void);
void test_Lexer_unexpected_character(void);

#endif // TEST_LEXER_H