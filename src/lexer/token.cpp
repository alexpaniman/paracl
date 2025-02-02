#include "paracl/lexer/token.h"


namespace paracl {

const char* get_token_type_name(token_type type) {
    switch (type) {
    case token_type::NONE:                return "NONE";

    case token_type::ID:                  return "ID";

    case token_type::NUMBER:              return "NUMBER";

    case token_type::IF:                  return "IF";
    case token_type::WHILE:               return "WHILE";

    case token_type::EQUAL:               return "EQUAL";
    case token_type::LESS:                return "LESS";
    case token_type::BIGGER:              return "BIGGER";
    case token_type::LESS_OR_EQUAL:       return "LESS_OR_EQUAL";
    case token_type::BIGGER_OR_EQUAL:     return "BIGGER_OR_EQUAL";

    case token_type::LEFT_PARENTHESIS:    return "LEFT_PARENTHESIS";
    case token_type::RIGHT_PARENTHESIS:   return "RIGHT_PARENTHESIS";

    case token_type::LEFT_CURLY_BRACKET:  return "LEFT_CURLY_BRACKET";
    case token_type::RIGHT_CURLY_BRACKET: return "RIGHT_CURLY_BRACKET";

    case token_type::PLUS:                return "PLUS";
    case token_type::MINUS:               return "MINUS";
    case token_type::MULTIPLY:            return "MULTIPLY";
    case token_type::DIVIDE:              return "DIVIDE";

    case token_type::ASSIGN:              return "ASSIGN";

    case token_type::PLUS_ASSIGN:         return "PLUS_ASSIGN";
    case token_type::MINUS_ASSIGN:        return "MINUS_ASSIGN";
    case token_type::MULTIPLY_ASSIGN:     return "MULTIPLY_ASSIGN";
    case token_type::DIVIDE_ASSIGN:       return "DIVIDE_ASSIGN";

    case token_type::SCAN:                return "SCAN";

    case token_type::SEMICOLON:           return "SEMICOLON";
    }
}

} // end namespace paracl

