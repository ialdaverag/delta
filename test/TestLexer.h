#ifndef TEST_LEXER_H
#define TEST_LEXER_H

#include "unity.h"
#include "../src/lexer.h"

void test_Lexer_init(void);
void test_Lexer_position_tracking(void);
void test_Lexer_EOF(void);
void test_Lexer_unknown(void);
void test_Lexer_whitespace(void);
void test_Lexer_one_char_token(void);
void test_Lexer_one_or_two_char_token(void);
void test_Lexer_identifier(void);
void test_Lexer_keyword(void);
void test_Lexer_number(void);
void test_Lexer_string(void);
void test_Lexer_comment(void);

#endif // TEST_LEXER_H