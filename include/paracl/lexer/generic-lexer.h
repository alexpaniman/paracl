#pragma once

#include "paracl/text/display.h"

#include <vector>
#include <span>
#include <cstdint>
#include <optional>


namespace paracl {

class generic_lexer {
public:
    using token_type = uint8_t;
    static inline constexpr token_type SKIP = 0;

    struct state {
        using id = uint8_t;
        const state::id *transitions;

        bool is_terminal;
        token_type token;

        template <typename enum_type>
        state(state::id *transitions, bool is_terminal, enum_type token):
            transitions(transitions),
            is_terminal(is_terminal),
            token(static_cast<token_type>(token)) {
        }

        state(state::id *transitions, bool is_terminal):
            transitions(transitions),
            is_terminal(is_terminal),
            token(SKIP) {
        }
    };

    struct token {
        token_type type;
        text_range range;
        std::span<char> text;
    };


    generic_lexer(const state *states, std::span<char> text);

    std::optional<token> tokenize_next();
    std::vector<token> tokenize(); // TODO: useless? better name: tokenize_rest

    std::optional<colored_text> make_error_report();

private:
    const state *states_;
    static inline constexpr int INITIAL_STATE = 30; // TODO: fix

    std::span<char>::iterator begin_, end_;
    std::span<char>::iterator iter_;
    text_position position_;

    state::id current_state_id_;
    token last_terminal_;

    std::vector<text_range> errors_;
    std::vector<token> tokens_; // TODO: lexer probably shouldn't store this, now it's only for error reporting

    std::optional<token> emit_token();

    void record_error_and_skip_it();
    void advance();
};

} // end namespace paracl

