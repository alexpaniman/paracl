#pragma once

#include "paracl/ast/nodes.h"
#include "paracl/lexer/lexer.h"

#include <string>
#include <vector>
#include <unordered_map>


namespace paracl {

class parser {
public:
    explicit parser(std::vector<token> tokens, context &ctx):
        tokens_(std::move(tokens)), context_(ctx) {}

    std::vector<std::unique_ptr<node>> parse() {
        return parse_scope();
    }

private:
    token eat_token();
    token next_token();
    token current_token();

    std::unique_ptr<node> parse_id_or_num(bool create_variable);
    std::unique_ptr<node> parse_function();
    std::unique_ptr<node> parse_arithmetic_op();
    std::unique_ptr<node> parse_comparison_op();
    std::unique_ptr<node> parse_expression();
    std::unique_ptr<node> parse_condition();
    std::vector<std::unique_ptr<node>> parse_scope();

    template <typename node_type>
    std::unique_ptr<node> parse_binary_operation() {
        std::unique_ptr<node> left = parse_id_or_num(false);
        eat_token();
        std::unique_ptr<node> right = parse_id_or_num(false);
        return std::make_unique<node_type>(std::move(left), std::move(right));
    }

    template <typename node_type>
    std::unique_ptr<node> parse_assing_operation() {
        if (current_token().type != token_type::ID) {
            //добавить обработку ошибки
        }
        std::unique_ptr<node> left = parse_id_or_num(true);
        eat_token();
        std::unique_ptr<node> right = parse_arithmetic_op();
        if (eat_token().type != token_type::SEMICOLON) {
            //добавить обработку ошибки
        }

        return std::make_unique<node_type>(std::move(left), std::move(right));
    }

private:
    std::vector<token> tokens_;
    size_t current_token_num_ = 0;

    context &context_;
};

} // end namespace paracl