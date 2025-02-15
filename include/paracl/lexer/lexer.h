#pragma once

#include "paracl/lexer/token.h"
#include "paracl/text/display.h"


namespace paracl {

struct token {
    token_type type;
    text_range range;
    union {
        std::span<char> id;
        int64_t number;
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


enum class message_type {
    NOTE,
    WARNING,
    ERROR
};

void display_tokens(file source, message_type type, std::string message, std::initializer_list<annotated_token> ranges);


} // end namespace paracl

