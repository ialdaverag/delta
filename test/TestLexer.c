#include "TestLexer.h"

void test_Lexer_init() {
    const char* source = "test";
    
    Lexer lexer;
    Lexer_init(&lexer, source);

    TEST_ASSERT_EQUAL_STRING(source, lexer.start);
    TEST_ASSERT_EQUAL_STRING(source, lexer.current);
    TEST_ASSERT_EQUAL_INT(1, lexer.line);
    TEST_ASSERT_EQUAL_INT(1, lexer.column);
}

static void test_Lexer_position_tracking(void) {
    {
        // inicio
        Lexer lexer;
        Lexer_init(&lexer, "var x = 10");

        TEST_ASSERT_EQUAL_STRING("var x = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING("var x = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(1, lexer.column);

        Token token;

        // var
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_VAR, token.type);
        TEST_ASSERT_EQUAL_STRING("var", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("var x = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING(" x = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(4, lexer.column);
        
        // x
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("x", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(5, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("x = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING(" = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(6, lexer.column);
        
        // =
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING("=", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(7, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("= 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING(" 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(8, lexer.column);
        
        // 10
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_INTEGER, token.type);
        TEST_ASSERT_EQUAL_STRING("10", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(9, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("10", lexer.start);
        TEST_ASSERT_EQUAL_STRING("", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(11, lexer.column);
    }
    {
        // Inicio
        Lexer lexer;
        Lexer_init(&lexer, "var x\nx = 10");

        TEST_ASSERT_EQUAL_STRING("var x\nx = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING("var x\nx = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(1, lexer.column);

        Token token;
        
        // var (primera línea)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_VAR, token.type);
        TEST_ASSERT_EQUAL_STRING("var", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("var x\nx = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING(" x\nx = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(4, lexer.column);
        
        // x (primera línea)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("x", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(5, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("x\nx = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING("\nx = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(6, lexer.column);

        // \n (primera línea)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NEWLINE, token.type);
        TEST_ASSERT_EQUAL_STRING("\n", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(6, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("\nx = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING("x = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(2, lexer.line);
        TEST_ASSERT_EQUAL_INT(1, lexer.column);
        
        // x (segunda línea)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("x", token.lexeme);
        TEST_ASSERT_EQUAL_INT(2, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("x = 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING(" = 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(2, lexer.line);
        TEST_ASSERT_EQUAL_INT(2, lexer.column);
        
        // = (segunda línea)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING("=", token.lexeme);
        TEST_ASSERT_EQUAL_INT(2, token.line);
        TEST_ASSERT_EQUAL_INT(3, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("= 10", lexer.start);
        TEST_ASSERT_EQUAL_STRING(" 10", lexer.current);
        TEST_ASSERT_EQUAL_INT(2, lexer.line);
        TEST_ASSERT_EQUAL_INT(4, lexer.column);

        // 10 (segunda línea)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_INTEGER, token.type);
        TEST_ASSERT_EQUAL_STRING("10", token.lexeme);
        TEST_ASSERT_EQUAL_INT(2, token.line);
        TEST_ASSERT_EQUAL_INT(5, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("10", lexer.start);
        TEST_ASSERT_EQUAL_STRING("", lexer.current);
        TEST_ASSERT_EQUAL_INT(2, lexer.line);
        TEST_ASSERT_EQUAL_INT(7, lexer.column);
    }
}

static void test_Lexer_next_token_EOF(void) {
    Lexer lexer;
    Lexer_init(&lexer, "");

    Token token = Lexer_next_token(&lexer);
    TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
    TEST_ASSERT_EQUAL_STRING("", token.lexeme);
    TEST_ASSERT_EQUAL_INT(1, token.line);
    TEST_ASSERT_EQUAL_INT(1, token.column);
    Token_free(&token);
}

static void test_Lexer_next_token_whitespace(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, " ");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
        TEST_ASSERT_EQUAL_STRING("", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(2, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "   ");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
        TEST_ASSERT_EQUAL_STRING("", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(4, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\r");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NL, token.type);
        TEST_ASSERT_EQUAL_STRING("\r", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\t");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
        TEST_ASSERT_EQUAL_STRING("", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(2, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\n");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NL, token.type);
        TEST_ASSERT_EQUAL_STRING("\n", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\r\t\n");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NL, token.type);
        TEST_ASSERT_EQUAL_STRING("\r", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Lexer_next_token_newline(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, "\n");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NL, token.type);
        TEST_ASSERT_EQUAL_STRING("\n", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\r\n");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NL, token.type);
        TEST_ASSERT_EQUAL_STRING("\r\n", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Lexer_next_token_one_char_token(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, "+");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_PLUS, token.type);
        TEST_ASSERT_EQUAL_STRING("+", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "-");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_MINUS, token.type);
        TEST_ASSERT_EQUAL_STRING("-", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "*");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_STAR, token.type);
        TEST_ASSERT_EQUAL_STRING("*", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "/");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_SLASH, token.type);
        TEST_ASSERT_EQUAL_STRING("/", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "%");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_PERCENT, token.type);
        TEST_ASSERT_EQUAL_STRING("%", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "()");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_LEFT_PARENTHESIS, token.type);
        TEST_ASSERT_EQUAL_STRING("(", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_RIGHT_PARENTHESIS, token.type);
        TEST_ASSERT_EQUAL_STRING(")", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(2, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "[]");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_LEFT_BRACKET, token.type);
        TEST_ASSERT_EQUAL_STRING("[", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_RIGHT_BRACKET, token.type);
        TEST_ASSERT_EQUAL_STRING("]", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(2, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "{}");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_LEFT_BRACE, token.type);
        TEST_ASSERT_EQUAL_STRING("{", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_RIGHT_BRACE, token.type);
        TEST_ASSERT_EQUAL_STRING("}", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(2, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, ",");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMA, token.type);
        TEST_ASSERT_EQUAL_STRING(",", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, ".");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_DOT, token.type);
        TEST_ASSERT_EQUAL_STRING(".", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, ":");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COLON, token.type);
        TEST_ASSERT_EQUAL_STRING(":", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Lexer_next_token_one_or_two_chars_token(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, "=");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING("=", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "==");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EQUAL_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING("==", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "!");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        TEST_ASSERT_EQUAL_STRING("!", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "!=");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_BANG_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING("!=", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "<");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_LESS, token.type);
        TEST_ASSERT_EQUAL_STRING("<", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "<=");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_LESS_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING("<=", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, ">");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_GREATER, token.type);
        TEST_ASSERT_EQUAL_STRING(">", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, ">=");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_GREATER_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING(">=", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Lexer_next_token_identifier(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, "variable");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("variable", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "var_123");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("var_123", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "var123");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("var123", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "_var123");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("_var123", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "miVariable");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("miVariable", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "mi_variable");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("mi_variable", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "VARIABLE");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("VARIABLE", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Lexer_next_token_keyword(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, "var");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_VAR, token.type);
        TEST_ASSERT_EQUAL_STRING("var", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "const");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_CONST, token.type);
        TEST_ASSERT_EQUAL_STRING("const", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "si");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_SI, token.type);
        TEST_ASSERT_EQUAL_STRING("si", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "sino");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_SINO, token.type);
        TEST_ASSERT_EQUAL_STRING("sino", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "segun");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_SEGUN, token.type);
        TEST_ASSERT_EQUAL_STRING("segun", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "caso");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_CASO, token.type);
        TEST_ASSERT_EQUAL_STRING("caso", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "otro");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_OTRO, token.type);
        TEST_ASSERT_EQUAL_STRING("otro", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "mientras");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_MIENTRAS, token.type);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "para");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_PARA, token.type);
        TEST_ASSERT_EQUAL_STRING("para", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "continuar");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_CONTINUAR, token.type);
        TEST_ASSERT_EQUAL_STRING("continuar", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "romper");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ROMPER, token.type);
        TEST_ASSERT_EQUAL_STRING("romper", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "fun");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_FUN, token.type);
        TEST_ASSERT_EQUAL_STRING("fun", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "ret");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_RET, token.type);
        TEST_ASSERT_EQUAL_STRING("ret", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "clase");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_CLASE, token.type);
        TEST_ASSERT_EQUAL_STRING("clase", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "y");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_Y, token.type);
        TEST_ASSERT_EQUAL_STRING("y", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "o");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_O, token.type);
        TEST_ASSERT_EQUAL_STRING("o", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "no");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NO, token.type);
        TEST_ASSERT_EQUAL_STRING("no", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "verdadero");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_VERDADERO, token.type);
        TEST_ASSERT_EQUAL_STRING("verdadero", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "falso");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_FALSO, token.type);
        TEST_ASSERT_EQUAL_STRING("falso", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "nulo");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NULO, token.type);
        TEST_ASSERT_EQUAL_STRING("nulo", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
}

static void test_Lexer_next_token_number(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, "123");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_INTEGER, token.type);
        TEST_ASSERT_EQUAL_STRING("123", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "123.456");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_FLOAT, token.type);
        TEST_ASSERT_EQUAL_STRING("123.456", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "0.123");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_FLOAT, token.type);
        TEST_ASSERT_EQUAL_STRING("0.123", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "123.");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        TEST_ASSERT_EQUAL_STRING("ERROR: Numero mal formado.", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column); // Donde detectó el error
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "123.45.67");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        TEST_ASSERT_EQUAL_STRING("ERROR: Numero mal formado.", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column); // Donde detectó el error
        Token_free(&token);
    }
}

static void test_Lexer_next_token_string(void) {
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"Hello, World!\"");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_STRING, token.type);
        TEST_ASSERT_EQUAL_STRING("\"Hello, World!\"", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "'Hello, World!'");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_STRING, token.type);
        TEST_ASSERT_EQUAL_STRING("'Hello, World!'", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"Hello\\nWorld\"");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_STRING, token.type);
        TEST_ASSERT_EQUAL_STRING("\"Hello\\nWorld\"", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"Hello\\tWorld\"");

        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_STRING, token.type);
        TEST_ASSERT_EQUAL_STRING("\"Hello\\tWorld\"", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"Esta cadena no termina");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"Cadena con\nsalto\"");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"Cadena con comillas mezcladas'");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_ERROR, token.type);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "\"Cadena con escape mal formado \\x\"");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_STRING, token.type);
        Token_free(&token);
    }
}

static void test_Lexer_next_token_comment() {
    {
        Lexer lexer;
        Lexer_init(&lexer, "#");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("#", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "#Comentario sin nueva línea");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("#Comentario sin nueva línea", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "   # Comentario con espacios antes");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("# Comentario con espacios antes", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(4, token.column); // Los 3 espacios + 1 para el #
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "# !@#$%^&*()_+-=[]{}\\|;:'\",.<>/?");
        
        Token token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("# !@#$%^&*()_+-=[]{}\\|;:'\",.<>/?", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "#Primer comentario\n#Segundo comentario\n#Tercer comentario");

        Token token;
        
        // Primer comentario
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("#Primer comentario", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("#Primer comentario\n#Segundo comentario\n#Tercer comentario", lexer.start);
        TEST_ASSERT_EQUAL_STRING("\n#Segundo comentario\n#Tercer comentario", lexer.current);
        TEST_ASSERT_EQUAL_INT(1, lexer.line);
        TEST_ASSERT_EQUAL_INT(19, lexer.column); // 19 caracteres + 1 para el #

        // Nueva línea (primer comentario)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NL, token.type);
        TEST_ASSERT_EQUAL_STRING("\n", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(19, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("\n#Segundo comentario\n#Tercer comentario", lexer.start);
        TEST_ASSERT_EQUAL_STRING("#Segundo comentario\n#Tercer comentario", lexer.current);
        TEST_ASSERT_EQUAL_INT(2, lexer.line);
        TEST_ASSERT_EQUAL_INT(1, lexer.column); // 1 para el #
        
        // Segundo comentario
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("#Segundo comentario", token.lexeme);
        TEST_ASSERT_EQUAL_INT(2, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("#Segundo comentario\n#Tercer comentario", lexer.start);
        TEST_ASSERT_EQUAL_STRING("\n#Tercer comentario", lexer.current);
        TEST_ASSERT_EQUAL_INT(2, lexer.line);
        TEST_ASSERT_EQUAL_INT(20, lexer.column); // 18 caracteres + 1 para el #

        // Nueva línea (segundo comentario)
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_NL, token.type);
        TEST_ASSERT_EQUAL_STRING("\n", token.lexeme);
        TEST_ASSERT_EQUAL_INT(2, token.line);
        TEST_ASSERT_EQUAL_INT(20, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("\n#Tercer comentario", lexer.start);
        TEST_ASSERT_EQUAL_STRING("#Tercer comentario", lexer.current);
        TEST_ASSERT_EQUAL_INT(3, lexer.line);
        TEST_ASSERT_EQUAL_INT(1, lexer.column); // 1 para el #
        
        // Tercer comentario
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("#Tercer comentario", token.lexeme);
        TEST_ASSERT_EQUAL_INT(3, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("#Tercer comentario", lexer.start);
        TEST_ASSERT_EQUAL_STRING("", lexer.current);
        TEST_ASSERT_EQUAL_INT(3, lexer.line);
        TEST_ASSERT_EQUAL_INT(19, lexer.column); // 17 caracteres + 1 para el #
        
        // EOF
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
        TEST_ASSERT_EQUAL_STRING("", token.lexeme);
        TEST_ASSERT_EQUAL_INT(3, token.line);
        TEST_ASSERT_EQUAL_INT(19, token.column);
        Token_free(&token);

        TEST_ASSERT_EQUAL_STRING("", lexer.start);
        TEST_ASSERT_EQUAL_STRING("#Primer comentario", lexer.current);
        TEST_ASSERT_EQUAL_INT(3, lexer.line);
        TEST_ASSERT_EQUAL_INT(20, lexer.column); // 19 caracteres + 1 para el # (del último comentario)
    }
    {
        Lexer lexer;
        Lexer_init(&lexer, "var x = 10 # Esto es un comentario");

        Token token;

        // var
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_VAR, token.type);
        TEST_ASSERT_EQUAL_STRING("var", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(1, token.column);
        Token_free(&token);

        // x
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER, token.type);
        TEST_ASSERT_EQUAL_STRING("x", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(5, token.column);
        Token_free(&token);

        // =
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EQUAL, token.type);
        TEST_ASSERT_EQUAL_STRING("=", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(7, token.column);
        Token_free(&token);

        // 10
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_INTEGER, token.type);
        TEST_ASSERT_EQUAL_STRING("10", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(9, token.column);
        Token_free(&token);

        // # Esto es un comentario
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_COMMENT, token.type);
        TEST_ASSERT_EQUAL_STRING("# Esto es un comentario", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(12, token.column);
        Token_free(&token);

        // EOF
        token = Lexer_next_token(&lexer);
        TEST_ASSERT_EQUAL(TOKEN_EOF, token.type);
        TEST_ASSERT_EQUAL_STRING("", token.lexeme);
        TEST_ASSERT_EQUAL_INT(1, token.line);
        TEST_ASSERT_EQUAL_INT(35, token.column);
        Token_free(&token);
    }
}

void test_Lexer_next_token(void) {
    test_Lexer_position_tracking();
    test_Lexer_next_token_EOF();
    test_Lexer_next_token_whitespace();
    test_Lexer_next_token_newline();
    test_Lexer_next_token_one_char_token();
    test_Lexer_next_token_one_or_two_chars_token();
    test_Lexer_next_token_identifier();
    test_Lexer_next_token_keyword();
    test_Lexer_next_token_number();
    test_Lexer_next_token_string();
    test_Lexer_next_token_comment();
}