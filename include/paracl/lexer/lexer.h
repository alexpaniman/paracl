#pragma once

#include "paracl/lexer/token.h"
#include "paracl/text/display.h"


namespace paracl {

struct token {
    token_type type;
    text_range range;
    union {
        std::span<char> id;
        uint64_t number;
    };
};


template <>
struct rngable<token> {
    static text_range to_range(const token &tok) { return tok.range; }
};

std::vector<token> tokenize(std::span<char> input);

std::string describe_token(token tok);
void print_tokens(std::span<token> tokens);


struct annotated_token {
    token token;
    std::string annotation;
};

} // end namespace paracl

