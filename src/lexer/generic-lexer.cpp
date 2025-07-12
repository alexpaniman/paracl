#include "paracl/lexer/generic-lexer.h"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <iostream>


namespace paracl {

generic_lexer::generic_lexer(const generic_lexer::state *states, std::span<char> text):
    states_(states),
    iter_(text.begin()), end_(text.end()),
    position_{/*point=*/0, /*line=*/1, /*column=*/0},
    current_state_id_(INITIAL_STATE),
    last_terminal_(SKIP, text) {

    assert(!states_[INITIAL_STATE].is_terminal);
}

auto generic_lexer::emit_token() -> std::optional<token> {
    if (last_terminal_.text.empty())
        return {};

    std::optional<token> result = std::nullopt;
    if (last_terminal_.type != SKIP)
        result = last_terminal_;

    last_terminal_.text = {
        last_terminal_.text.end(),
        last_terminal_.text.end()
    };

    iter_ = last_terminal_.text.end();
    current_state_id_ = INITIAL_STATE;

    return result;
}

auto generic_lexer::advance() -> void {
    assert(iter_ != end_);
    ++ iter_;

    if (iter_ == end_)
        return;

    ++ position_.column;
    if (*iter_ == '\n') {
        position_.line ++;
        position_.column = 0;
    }
}

auto generic_lexer::tokenize_next() -> std::optional<token> {
    while (iter_ != end_) {
        const state::id *transitions = states_[current_state_id_].transitions;

        state::id next_state_id = transitions[static_cast<uint8_t>(*iter_)];
        state next_state = states_[next_state_id];

        if (!next_state.transitions) {
            state::id previous_state = current_state_id_;

            if (std::optional<token> emitted = emit_token())
                return *emitted;

            // Nothing changed, we're in an infinite loop
            if (previous_state == current_state_id_)
                throw std::runtime_error("error");

            continue;
        }

        advance();
        current_state_id_ = next_state_id;

        if (next_state.is_terminal)
            last_terminal_ = {
                .type = next_state.token,
                .text = std::span<char>{
                    last_terminal_.text.begin(),
                    iter_
                }
            };
    }

    return emit_token();
}

auto generic_lexer::tokenize() -> std::vector<token> {
    std::vector<token> tokens;
    while (std::optional<token> emitted = tokenize_next())
        tokens.push_back(*emitted);

    return tokens;
}

} // end namespace paracl
