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
            uint64_t number = std::stoull(stringified_number);

            tokens.push_back({type, tok->range, {.number = number}});
            continue;
        }

        tokens.push_back({type, tok->range, {}});
    }

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

inline constexpr std::string RESET   = "\033[0m";
inline constexpr std::string RED     = "\033[31m";
inline constexpr std::string GREEN   = "\033[32m";
inline constexpr std::string YELLOW  = "\033[33m";
inline constexpr std::string BLUE    = "\033[34m";
inline constexpr std::string MAGENTA = "\033[35m";
inline constexpr std::string CYAN    = "\033[36m";
inline constexpr std::string BOLD    = "\033[1m";

// Function to print messages with colors
void print_message(const std::string& type, const std::string& message, const std::string& color) {
    std::cout << BOLD << color << type << RESET << ": " << message << std::endl;
}

void display_tokens(file source, message_type type, std::string annotation, std::span<token> tokens) {
    assert(!tokens.empty());

    std::vector<text_range> locations;
    for (const token &tok: tokens)
        locations.push_back(tok.range);

    std::sort(locations.begin(), locations.end());

    std::cout << source.filename
        << ":" << tokens[0].range.x0.line
        << ":" << tokens[tokens.size() - 1].range.x1.line
        << " ";

    switch (type) {
    case message_type::NOTE:    print_message("note",    annotation, CYAN);   break;
    case message_type::ERROR:   print_message("error",   annotation, RED);    break;
    case message_type::WARNING: print_message("warning", annotation, YELLOW); break;
    }

    print_range(source.text, std::move(locations));
}

} // end namespace paracl

