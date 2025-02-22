#include "paracl/parser/parser.h"


namespace paracl {

token parser::eat_token() {
    if (current_token_num_ >= tokens_.size()) {
        //добавить обработку ошибки
    }

    token current_token = tokens_[current_token_num_];
    ++ current_token_num_;
    if (current_token.type == token_type::NONE) {
        //добавить обработку ошибки
    }

    return current_token;
}

token parser::current_token() const {
    if (current_token_num_ >= tokens_.size()) {
        //добавить обработку ошибки
    }
    return tokens_[current_token_num_];
}

token parser::next_token() const {
    if (current_token_num_ + 1 >= tokens_.size()) {
        //добавить обработку ошибки
    }
    return tokens_[current_token_num_ + 1];
}

int parser::get_operator_precedence(token_type type) const {
    switch (type) {
        case token_type::PLUS:
        case token_type::MINUS:
            return 1;
        case token_type::MULTIPLY:
        case token_type::DIVIDE:
            return 2;
        default:
            return 0; //обработку
    }
}

std::unique_ptr<node> parser::parse_expression(int min_precedence) {
    std::unique_ptr<node> left = parse_id_or_num();

    while (current_token().type != token_type::RIGHT_PARENTHESIS &&
           current_token().type != token_type::SEMICOLON) {
        token current = current_token();
        int precedence = get_operator_precedence(current.type);

        if (precedence < min_precedence) {
            return left;
        }

        eat_token();
        std::unique_ptr<node> right = parse_expression(precedence + 1);

        switch (current.type) {
            case token_type::PLUS:
                left = std::make_unique<plus_node>(std::move(left), std::move(right));
                break;
            case token_type::MINUS:
                left = std::make_unique<minus_node>(std::move(left), std::move(right));
                break;
            case token_type::MULTIPLY:
                left = std::make_unique<multiply_node>(std::move(left), std::move(right));
                break;
            case token_type::DIVIDE:
                left = std::make_unique<divide_node>(std::move(left), std::move(right));
                break;
            case token_type::EQUAL:
                left = std::make_unique<equal_node>(std::move(left), std::move(right));
                break;
            case token_type::LESS:
                left = std::make_unique<less_node>(std::move(left), std::move(right));
                break;
            case token_type::BIGGER:
                left = std::make_unique<bigger_node>(std::move(left), std::move(right));
                break;
            case token_type::LESS_OR_EQUAL:
                left = std::make_unique<less_or_equal_node>(std::move(left), std::move(right));
                break;
            case token_type::BIGGER_OR_EQUAL:
                left = std::make_unique<bigger_or_equal_node>(std::move(left), std::move(right));
                break;
            default:
                // добавить обработку ошибки
                return nullptr;
        }
    }
    return left;
}

std::unique_ptr<node> parser::parse_id_or_num(bool create_variable) {
    token current_token = eat_token();

    if (current_token.type == token_type::LEFT_PARENTHESIS) {
        std::unique_ptr<node> expr = parse_expression(0);
        if (eat_token().type != token_type::RIGHT_PARENTHESIS) {
            // добавить обработку ошибки
        }
        return expr;
    }

    bool is_neg = false;
    if(current_token.type == token_type::MINUS) {
        is_neg = true;
        current_token = eat_token();
    }

    switch(current_token.type) {
        case token_type::ID: {
            std::string id_name{current_token.id.data(), current_token.id.size()};

            if (!create_variable && !context_.check_var_existing(id_name)) {
                //обработка
            }

            context_.create_variable(id_name);
            std::unique_ptr<node> new_id_node = std::make_unique<id_node>(id_name);
            if (is_neg) {
                return std::make_unique<negate_node>(std::move(new_id_node));
            }
            return new_id_node;
        }
        case token_type::NUMBER: {
            std::unique_ptr<node> new_num_node = std::make_unique<number_node>(current_token.number);
            if (is_neg) {
                return std::make_unique<negate_node>(std::move(new_num_node));
            }
            return new_num_node;
        }
        case token_type::SCAN: {
            std::unique_ptr<node> new_scan_node = std::make_unique<scan_node>();
            if (is_neg) {
                return std::make_unique<negate_node>(std::move(new_scan_node));
            }
            return new_scan_node;
        }
        default:
            return nullptr; //добавить обработку ошибки
    }
}

std::unique_ptr<node> parser::parse_function() {
    token function = eat_token();
    if (function.type != token_type::ID) {
        //обработка ошибки
    }
    std::string function_name{function.id.data(), function.id.size()};

    eat_token();
    std::vector<std::unique_ptr<node>> args;
    while (current_token().type != token_type::RIGHT_PARENTHESIS) {
        args.push_back(parse_expression(0));

        //надо будет добавить запятую
        //if (eat_token().type !=token_type::COMMA)
    }
    eat_token();
    return std::make_unique<function_node>(function_name, std::move(args));
}

std::unique_ptr<node> parser::parse_assing_operation() {
    if (current_token().type != token_type::ID) {
        //добавить обработку ошибки
    }
    switch(next_token().type) {
        case token_type::ASSIGN:           return parse_binary_operation<assign_node>(true);
        case token_type::PLUS_ASSIGN:      return parse_binary_operation<plus_assign_node>(true);
        case token_type::MINUS_ASSIGN:     return parse_binary_operation<minus_assign_node>(true);
        case token_type::MULTIPLY_ASSIGN:  return parse_binary_operation<multiply_assign_node>(true);
        case token_type::DIVIDE_ASSIGN:    return parse_binary_operation<divide_assign_node>(true);
        case token_type::LEFT_PARENTHESIS: return parse_function();
        default:                           return nullptr;  //добавить обработку ошибки
    }
}

std::unique_ptr<node> parser::parse_condition() {
    if (eat_token().type != token_type::LEFT_PARENTHESIS) {
        //добавить обработку ошибки
    }
    std::unique_ptr<node> condition = parse_expression(0);
    if (eat_token().type != token_type::RIGHT_PARENTHESIS) {
        //добавить обработку ошибки
    }
    return condition;
}

std::vector<std::unique_ptr<node>> parser::parse_scope() {
    std::vector<std::unique_ptr<node>> scope;

    while (current_token_num_ != tokens_.size()
           && current_token().type != token_type::RIGHT_CURLY_BRACKET) {
        switch(current_token().type) {
            case token_type::ID:
                scope.push_back(parse_assing_operation());
                if (eat_token().type != token_type::SEMICOLON) {
                    //добавить обработку ошибки
                }
                break;

            case token_type::WHILE:
            case token_type::IF: {
                token keyword = eat_token();
                std::unique_ptr<node> condition = parse_condition();

                if (eat_token().type != token_type::LEFT_CURLY_BRACKET) return {}; // добавить обработку ошибки
                std::vector<std::unique_ptr<node>> body = parse_scope();
                if (eat_token().type != token_type::RIGHT_CURLY_BRACKET) return {}; // добавить обработку ошибки

                if (keyword.type == token_type::WHILE) {
                    std::unique_ptr<node> while_n = std::make_unique<while_node>(std::move(condition),
                                                                                 std::move(body));
                    scope.push_back(std::move(while_n));
                }
                else if (keyword.type == token_type::IF) {
                    std::unique_ptr<node> if_n = std::make_unique<if_node>(std::move(condition),
                                                                           std::move(body));
                    scope.push_back(std::move(if_n));
                }
                else {
                    // добавить обработку ошибки
                }

                break;
            }

            default:
                return {}; // добавить обработку ошибки
        }
    }
    return scope;
}

} // end namespace paracl