#include "paracl/lexer/generic-lexer.h"
#include "paracl/lexer/token.h"
#include "paracl/text/colored-text.h"
#include "paracl/text/display.h"
#include "paracl/text/text-annotator.h"

#include <cassert>
#include <stdexcept>
#include <vector>


namespace paracl {

generic_lexer::generic_lexer(const generic_lexer::state *states, std::span<char> text):
    states_(states),
    begin_(text.begin()), end_(text.end()),
    iter_(text.begin()),
    position_{/*point=*/0, /*line=*/1, /*column=*/0},
    current_state_id_(INITIAL_STATE),
    last_terminal_(SKIP, {position_, position_}, text) {

    assert(!states_[INITIAL_STATE].is_terminal);
}

auto generic_lexer::emit_token() -> std::optional<token> {
    if (last_terminal_.text.empty())
        return std::nullopt;

    token result = last_terminal_;

    last_terminal_.text = {
        last_terminal_.text.end(),
        last_terminal_.text.end()
    };

    iter_ = last_terminal_.text.end();
    current_state_id_ = INITIAL_STATE;

    last_terminal_.range.begin = position_;

    tokens_.emplace_back(result);
    if (last_terminal_.type == SKIP)
        return std::nullopt;

    return result;
}

auto generic_lexer::advance() -> void {
    assert(iter_ != end_);

    if (iter_ == end_)
        return;

    ++ position_.point;

    auto prev_iter = iter_ ++;
    if (*prev_iter == '\n') {
        position_.line ++;
        position_.column = 0;

        return;
    }

    ++ position_.column;
}


auto generic_lexer::record_error_and_skip_it() -> void {
    text_position error_position = position_;
    advance();

    if (errors_.empty() || errors_.back().end != error_position) {
        errors_.emplace_back(error_position, position_);
        return;
    }

    errors_.back() = {
        errors_.back().begin,
        position_
    };
}

auto generic_lexer::tokenize_next() -> std::optional<token> {
    while (iter_ != end_) {
        const state::id *transitions = states_[current_state_id_].transitions;

        state::id next_state_id = transitions[static_cast<int>(*iter_)];
        state next_state = states_[next_state_id];

        if (!next_state.transitions) {
            state::id previous_state = current_state_id_;

            if (std::optional<token> emitted = emit_token())
                return *emitted;

            // Nothing changed, we're in an infinite loop
            if (previous_state == current_state_id_)
                record_error_and_skip_it();

            continue;
        }

        advance();
        current_state_id_ = next_state_id;

        if (next_state.is_terminal)
            last_terminal_ = {
                .type = next_state.token,
                .range = text_range{last_terminal_.range.begin, position_},
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
    
auto generic_lexer::make_error_report() -> std::optional<colored_text> {
    if (errors_.empty())
        return std::nullopt;

    std::vector<annotated_range> ranges;

    for (text_range error: errors_) {
        ranges.emplace_back(error, "unexpected character(s)");
    }

    for (token current_token: tokens_) {
        if (current_token.type != 0) // TODO: 0?
            ranges.emplace_back(current_token.range, "");
    }

    return annotate(std::span<char>{begin_, end_}, std::move(ranges));
}

} // end namespace paracl
