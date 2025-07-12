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
    token next_token() const;
    token current_token() const;

    int get_operator_precedence(token_type type) const;

    std::unique_ptr<node> parse_id_or_num(/*bool create_variable = false*/);
    std::unique_ptr<node> parse_expression(int min_precedence);
    std::unique_ptr<node> parse_function();
    std::unique_ptr<node> parse_comparison_operation();
    std::unique_ptr<node> parse_assing_operation();
    std::unique_ptr<node> parse_condition();
    std::vector<std::unique_ptr<node>> parse_scope();

    template <typename node_type>
    std::unique_ptr<node> parse_binary_operation(/*bool create_var = false*/) {
        std::unique_ptr<node> left = parse_id_or_num(/*create_var*/);
        eat_token();
        std::unique_ptr<node> right = parse_expression(0);
        return std::make_unique<node_type>(std::move(left), std::move(right));
    }

private:
    std::vector<token> tokens_;
    size_t current_token_num_ = 0;

    context &context_;
};

} // end namespace paracl