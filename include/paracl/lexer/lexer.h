#pragma once

#include "paracl/lexer/token.h"


namespace paracl {

struct token {
    token_type type;
    union {
        std::span<char> id;
        int64_t number;
    };
};

std::vector<token> tokenize(std::span<char> input);

std::string describe_token(token tok);
void print_tokens(const std::span<token> &tokens);

} // end namespace paracl

