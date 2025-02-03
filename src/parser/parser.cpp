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

token parser::current_token() {
    if (current_token_num_ >= tokens_.size()) {
        //добавить обработку ошибки
    }
    return tokens_[current_token_num_];
}

token parser::next_token() {
    if (current_token_num_ + 1 >= tokens_.size()) {
        //добавить обработку ошибки
    }
    return tokens_[current_token_num_ + 1];
}

std::unique_ptr<node> parser::parse_id_or_num(bool create_variable) {
    token current_token = eat_token();
    switch(current_token.type) {
        case token_type::ID: {
            std::string id_name{current_token.id.data(), current_token.id.size()};

            if (!create_variable && !context_.check_var_existing(id_name)) {
                //обработка
            }

            context_.create_variable(id_name);
            return std::make_unique<id_node>(id_name);
        }
        case token_type::NUMBER:
            return std::make_unique<number_node>(current_token.number);
        case token_type::SCAN:
            return std::make_unique<Scan>();
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
        args.push_back(parse_id_or_num(false));

        //надо будет добавить запятую
        //if (eat_token().type !=token_type::COMMA)
    }
    eat_token();

    if (eat_token().type != token_type::SEMICOLON) {
        //добавить обработку ошибки
    }

    return std::make_unique<function_node>(function_name, std::move(args));
}

std::unique_ptr<node> parser::parse_arithmetic_op() {
    switch(next_token().type) {
        case token_type::PLUS:      return parse_binary_operation<plus_node>();
        case token_type::MINUS:     return parse_binary_operation<minus_node>();
        case token_type::MULTIPLY:  return parse_binary_operation<multiply_node>();
        case token_type::DIVIDE:    return parse_binary_operation<divide_node>();
        case token_type::SEMICOLON: return parse_id_or_num(false);
        default:                    return nullptr; //добавить обработку ошибки
    }
}

std::unique_ptr<node> parser::parse_comparison_op() {
    switch(next_token().type) {
        case token_type::EQUAL:           return parse_binary_operation<equal_node>();
        case token_type::LESS:            return parse_binary_operation<less_node>();
        case token_type::BIGGER:          return parse_binary_operation<bigger_node>();
        case token_type::LESS_OR_EQUAL:   return parse_binary_operation<less_or_equal_node>();
        case token_type::BIGGER_OR_EQUAL: return parse_binary_operation<bigger_or_equal_node>();
        default:                          return nullptr; //добавить обработку ошибки
    }
}

std::unique_ptr<node> parser::parse_expression() {
    switch(next_token().type) {
        case token_type::ASSIGN:           return parse_assing_operation<assign_node>();
        case token_type::PLUS_ASSIGN:      return parse_assing_operation<plus_assign_node>();
        case token_type::MINUS_ASSIGN:     return parse_assing_operation<minus_assign_node>();
        case token_type::MULTIPLY_ASSIGN:  return parse_assing_operation<multiply_assign_node>();
        case token_type::DIVIDE_ASSIGN:    return parse_assing_operation<divide_assign_node>();
        case token_type::LEFT_PARENTHESIS: return parse_function();
        default:                           return nullptr;  //добавить обработку ошибки
    }
}

std::unique_ptr<node> parser::parse_condition() {
    if (eat_token().type != token_type::LEFT_PARENTHESIS) {
        //добавить обработку ошибки
    }
    std::unique_ptr<node> condition = parse_comparison_op();
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
                scope.push_back(parse_expression());
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