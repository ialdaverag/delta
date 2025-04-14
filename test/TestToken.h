#ifndef TEST_TOKEN_H
#define TEST_TOKEN_H

#include <string.h>

#include "unity.h"
#include "Token.h"

void test_Token_init_EOF(void);
void test_Token_init_unknown(void);
void test_Token_init_error(void);
void test_Token_init_one_char(void);
void test_Token_init_one_or_two_chars(void);
void test_Token_init_identifiers(void);
void test_Token_init_keywords(void);
void test_Token_init_strings(void);
void test_Token_init_numbers(void);
void test_Token_init_comments(void);
void test_Token_free(void);

#endif