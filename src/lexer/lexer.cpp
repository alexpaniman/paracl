#include "paracl/lexer/lexer.h"
#include "paracl/lexer/generic-lexer.h"
#include "paracl/lexer/token.h"

#include "paracl/text/display.h"
#include "paracl/text/ansi.h"

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

// Function to print messages with colors
void print_message(const std::string& type, const std::string& message, const std::string& color) {
    std::cout << BOLD << color << type << RESET << ": " << message << std::endl;
}


void display_tokens(file source, message_type type, std::string message, std::initializer_list<annotated_token> ranges) {
    assert(ranges.size() != 0);

    std::vector<annotated_range> annotated_ranges;
    for (const auto &[tok, annotation]: ranges)
        annotated_ranges.push_back({tok.range, annotation});

    auto &begin_range = annotated_ranges[0].range.begin;

    std::cout << source.filename << ":" << begin_range.line << ":" << begin_range.column << " ";

    switch (type) {
    case message_type::NOTE:    print_message("note",    message, CYAN);   break;
    case message_type::ERROR:   print_message("error",   message, RED);    break;
    case message_type::WARNING: print_message("warning", message, YELLOW); break;
    }


    annotation_config cfg {
        .line = { .foreground_color = ansi_preset_color::GREEN }
    };

    colored_text_stream stream;
    print_range(stream, source.text, std::move(annotated_ranges), cfg);

    stream.print();
}

} // end namespace paracl

