#include "paracl/lexer/lexer.h"
#include "paracl/lexer/generic-lexer.h"
#include "paracl/lexer/token.h"

#include "paracl/text/display.h"
#include "state-machine.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>


namespace paracl {

std::vector<token> tokenize(std::span<char> input) {
    std::vector<token> tokens;

    generic_lexer lexer{lexer_states, input};
    while (std::optional<generic_lexer::token> tok = lexer.tokenize_next()) {
        static_assert(token_type::NONE == static_cast<token_type>(generic_lexer::SKIP));

        token_type type = static_cast<token_type>(tok->type);
        assert(type != token_type::NONE && "token_type::NONE should be skipped");

        if (type == token_type::ID) {
            tokens.push_back({type, tok->range, {.id = tok->text}});
            continue;
        }

        if (type == token_type::NUMBER) {
            std::string stringified_number(tok->text.data(), tok->text.size());
            int64_t number = std::stoull(stringified_number);

            tokens.push_back({type, tok->range, {.number = number}});
            continue;
        }

        tokens.push_back({type, tok->range, {}});
    }

    std::optional<colored_text> error_report = lexer.make_error_report();
    if (error_report)
        error_report->print();

    return tokens;
}

std::string describe_token(token tok) {
    std::stringstream ss;
    ss << get_token_type_name(tok.type);

    if (tok.type == token_type::NUMBER)
        ss << "{" << tok.number << "}";

    if (tok.type == token_type::ID)
        ss << "{" << std::string(tok.id.begin(), tok.id.end()) << "}";

    return ss.str();
}

void print_tokens(std::span<token> tokens) {
    for (const token &tok: tokens)
        std::cout << describe_token(tok) << "\n";
}

} // end namespace paracl

