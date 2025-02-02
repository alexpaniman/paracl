#pragma once

#include "paracl/lexer/generic-lexer.h"


namespace paracl {

enum class token_type: generic_lexer::token_type {
    NONE,

    ID,

    NUMBER,

    IF,
    WHILE,

    EQUAL,
    LESS,
    BIGGER,
    LESS_OR_EQUAL,
    BIGGER_OR_EQUAL,

    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,

    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    ASSIGN,

    PLUS_ASSIGN,
    MINUS_ASSIGN,
    MULTIPLY_ASSIGN,
    DIVIDE_ASSIGN,

    SCAN,

    SEMICOLON
};

const char* get_token_type_name(token_type type);

} // end namespace paracl

