#include "TestToken.h"

static void test_Token_init_EOF() {
    Token token;
    const char* lexeme = "\0";

    Token_init(&token, TOKEN_EOF, lexeme, lexeme + strlen(lexeme), 1, 1);
    TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
    TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
    TEST_ASSERT_EQUAL_INT(1, token.line);
    TEST_ASSERT_EQUAL_INT(1, token.column);
    Token_free(&token);
}

static void test_Token_init_error() {
    Token token;
    const char* lexeme = "error";

    Token_init(&token, TOKEN_ERROR, lexeme, lexeme + strlen(lexeme), 1, 1);
    TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
    TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
    TEST_ASSERT_EQUAL_INT(1, token.line);
    TEST_ASSERT_EQUAL_INT(1, token.column);
    Token_free(&token);
}

static void test_Token_init_one_char() {
    {
        Token token;
        const char* lexeme = "+";

        Token_init(&token, TOKEN_PLUS, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_PLUS, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "-";

        Token_init(&token, TOKEN_MINUS, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_MINUS, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "*";

        Token_init(&token, TOKEN_STAR, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_STAR, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "/";

        Token_init(&token, TOKEN_SLASH, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_SLASH, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "%";

        Token_init(&token, TOKEN_PERCENT, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_PERCENT, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "(";

        Token_init(&token, TOKEN_LEFT_PARENTHESIS, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_LEFT_PARENTHESIS, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = ")";
        
        Token_init(&token, TOKEN_RIGHT_PARENTHESIS, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_RIGHT_PARENTHESIS, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "{";

        Token_init(&token, TOKEN_LEFT_BRACE, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_LEFT_BRACE, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "}";

        Token_init(&token, TOKEN_RIGHT_BRACE, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_RIGHT_BRACE, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "[";

        Token_init(&token, TOKEN_LEFT_BRACKET, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_LEFT_BRACKET, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "]";

        Token_init(&token, TOKEN_RIGHT_BRACKET, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_RIGHT_BRACKET, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = ",";

        Token_init(&token, TOKEN_COMMA, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_COMMA, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = ".";

        Token_init(&token, TOKEN_DOT, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_DOT, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = ":";

        Token_init(&token, TOKEN_COLON, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_COLON, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Token_init_one_or_two_chars() {
    {
        Token token;
        const char* lexeme = "=";

        Token_init(&token, TOKEN_EQUAL, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "==";

        Token_init(&token, TOKEN_EQUAL_EQUAL, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_EQUAL_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "!";

        Token_init(&token, TOKEN_ERROR, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "!=";

        Token_init(&token, TOKEN_BANG_EQUAL, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_BANG_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "<";

        Token_init(&token, TOKEN_LESS, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_LESS, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "<=";

        Token_init(&token, TOKEN_LESS_EQUAL, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_LESS_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = ">";

        Token_init(&token, TOKEN_GREATER, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_GREATER, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = ">=";

        Token_init(&token, TOKEN_GREATER_EQUAL, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_GREATER_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Token_init_identifier() {
    {
        const char* lexeme = "var1";

        Token token;
        Token_init(&token, TOKEN_IDENTIFIER, lexeme, lexeme + strlen(lexeme), 1, 1);

        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);

        Token_free(&token);
    }
    {
        const char* lexeme = "var_2";

        Token token;
        Token_init(&token, TOKEN_IDENTIFIER, lexeme, lexeme + strlen(lexeme), 1, 1);

        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);

        Token_free(&token);
    }
    {
        const char* lexeme = "_var3";

        Token token;
        Token_init(&token, TOKEN_IDENTIFIER, lexeme, lexeme + strlen(lexeme), 1, 1);

        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);

        Token_free(&token);
    }
}

static void test_Token_init_keyword() {
    {
        Token token;
        const char* lexeme = "var";

        Token_init(&token, TOKEN_IDENTIFIER, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "const";

        Token_init(&token, TOKEN_CONST, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_CONST, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "si";

        Token_init(&token, TOKEN_SI, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_SI, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "sino";

        Token_init(&token, TOKEN_SINO, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_SINO, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "segun";

        Token_init(&token, TOKEN_SEGUN, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_SEGUN, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "caso";

        Token_init(&token, TOKEN_CASO, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_CASO, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "otro";

        Token_init(&token, TOKEN_OTRO, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_OTRO, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "para";

        Token_init(&token, TOKEN_PARA, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_PARA, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "mientras";

        Token_init(&token, TOKEN_MIENTRAS, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_MIENTRAS, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "continuar";

        Token_init(&token, TOKEN_CONTINUAR, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_CONTINUAR, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "romper";

        Token_init(&token, TOKEN_ROMPER, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_ROMPER, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "fun";

        Token_init(&token, TOKEN_FUN, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_FUN, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "ret";

        Token_init(&token, TOKEN_RET, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_RET, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "clase";

        Token_init(&token, TOKEN_CLASE, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_CLASE, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "y";

        Token_init(&token, TOKEN_Y, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_Y, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "o";

        Token_init(&token, TOKEN_O, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_O, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "no";

        Token_init(&token, TOKEN_NO, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_NO, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "verdadero";

        Token_init(&token, TOKEN_VERDADERO, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_VERDADERO, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "falso";

        Token_init(&token, TOKEN_FALSO, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_FALSO, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "nulo";

        Token_init(&token, TOKEN_NULO, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_NULO, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Token_init_string() {
    {
        Token token;
        const char* lexeme = "\"Hola, mundo!\"";

        Token_init(&token, TOKEN_STRING, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_STRING, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "'Hola, mundo!'";

        Token_init(&token, TOKEN_STRING, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_STRING, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Token_init_number() {
    {
        Token token;
        const char* lexeme = "123";

        Token_init(&token, TOKEN_INTEGER, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_INTEGER, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Token token;
        const char* lexeme = "3.1415";

        Token_init(&token, TOKEN_FLOAT, lexeme, lexeme + strlen(lexeme), 1, 1);
        TEST_ASSERT_EQUAL(TOKEN_FLOAT, token.type);
        TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }    
}

static void test_Token_init_comment() {
    Token token;
    const char* lexeme = "# Esto es un comentario";

    Token_init(&token, TOKEN_COMMENT, lexeme, lexeme + strlen(lexeme), 1, 1);
    TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
    TEST_ASSERT_EQUAL_STRING(lexeme, token.lexeme);
    TEST_ASSERT_EQUAL_INT(1, token.line);
    TEST_ASSERT_EQUAL_INT(1, token.column);
    Token_free(&token);
}

void test_Token_init() {
    test_Token_init_EOF();
    test_Token_init_error();
    test_Token_init_one_char();
    test_Token_init_one_or_two_chars();
    test_Token_init_identifier();
    test_Token_init_keyword();
    test_Token_init_string();
    test_Token_init_number();
    test_Token_init_comment();
}

void test_Token_free() {
    Token token;
    const char* lexeme = "var1";

    Token_init(&token, TOKEN_IDENTIFIER, lexeme, lexeme + strlen(lexeme), 1, 1);
    TEST_ASSERT_NOT_NULL(token.lexeme);
    Token_free(&token);
    TEST_ASSERT_NULL(token.lexeme);
}