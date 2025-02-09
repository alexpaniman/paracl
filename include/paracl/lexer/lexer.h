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

std::vector<token> tokenize(std::span<char> input);

std::string describe_token(token tok);
void print_tokens(std::span<token> tokens);


enum class message_type {
    NOTE,
    WARNING,
    ERROR
};

void display_tokens(file source, message_type type, std::string annotation, std::span<token> tokens);


} // end namespace paracl

