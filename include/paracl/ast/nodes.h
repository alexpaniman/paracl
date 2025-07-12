#pragma once

#include "paracl/ast/context.h"
#include "paracl/ast/marked_pointers.h"
#include "paracl/ast/graphviz_utils.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>


namespace paracl {

enum class node_type {
    NUMBER,
    ID,
    FUNCTION,
    ASSIGN,
    PLUSASSIGN,
    MINUSASSIGN,
    MULTIPLYASSIGN,
    DIVIDEASSIGN,
    NEGATE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQUAL,
    LESS,
    BIGGER,
    LESSEQUAL,
    BIGGEREQUAL,
    IF,
    WHILE,
    SCAN,
};

class node {
public:
    virtual node_type get_type() const noexcept = 0;
    virtual int64_t execute(context &ctx) = 0;
    virtual void to_program(std::ostream &ostr) const = 0;
    virtual void dump_gv(graphviz &graph, node_proxy& parent) const = 0;
    virtual void dump(std::ostream &ostr) const = 0;
    virtual ~node() = default;
};

class number_node final: public node {
public:
    explicit number_node(int64_t value):
        value_(value) {}

    node_type get_type() const noexcept override {
        return node_type::NUMBER;
    }

    int64_t execute([[maybe_unused]] context &ctx) override {
        return create_value(value_);
    }

    virtual void to_program(std::ostream &ostr) const override {
        ostr << value_;
    }

    void dump(std::ostream &ostr) const override {
        ostr << value_;
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        auto node = graph.insert_node(graphviz_formatter::number, std::to_string(value_));
        parent.connect(graphviz_formatter::default_edge, node);
    }

private:
    int64_t value_;
};


class id_node final: public node {
public:
    explicit id_node(std::string name):
        name_(std::move(name)) {}

    node_type get_type() const noexcept override {
        return node_type::ID;
    }

    int64_t execute(context &ctx) override {
        return create_pointer(ctx.get_variable(name_));
    }

    virtual void to_program(std::ostream &ostr) const override {
        ostr << name_;
    }

    void dump(std::ostream &ostr) const override {
        ostr << name_;
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        auto node = graph.insert_node(graphviz_formatter::id, std::move(name_));
        parent.connect(graphviz_formatter::default_edge, node);
    }

private:
    std::string name_;
};

class function_node final: public node {
public:
    explicit function_node(std::string name, std::vector<std::unique_ptr<node>> args):
        name_(std::move(name)), args_(std::move(args)) {}

    node_type get_type() const noexcept override {
        return node_type::FUNCTION;
    }

    int64_t execute(context &ctx) override {
        if (name_ == "print") {
            bool first = true;
            for (const auto& arg : args_) {
                if (!first) {
                    std::cout << " ";
                }
                std::cout << get_value(arg->execute(ctx));
                first = false;
            }
            std::cout << std::endl;
            return 1;
        }
        return 0; //остальные функции пока не реализованы
    }

    virtual void to_program(std::ostream &ostr) const override {
        ostr << name_ << "(";
        for (auto it = args_.begin(); it != args_.end(); ++it) {
            (*it)->to_program(ostr);
            if (std::next(it) != args_.end()) {
                ostr << ", ";
            }
        }
        ostr << ");";
    }

    void dump(std::ostream &ostr) const override {
        ostr << name_ << "( ";
        for (const auto& i: args_) {
            i->dump(ostr);
            ostr << " ";
        }
        ostr << ")";
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        auto node = graph.insert_node(graphviz_formatter::function, std::move(name_));
        parent.connect(graphviz_formatter::default_edge, node);

        for (const auto& i: args_) {
            i->dump_gv(graph, node);
        }
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<node>> args_;
};

template<typename impl_type>
class assign_operation: public node {
public:
    explicit assign_operation(std::unique_ptr<node> left, std::unique_ptr<node> right):
        left_(std::move(left)), right_(std::move(right)) {}
    virtual ~assign_operation() = default;

    int64_t assigned_value(context &ctx) const {
        return static_cast<const impl_type*>(this)->assigned_value(ctx);
    }

    node_type get_type() const noexcept override {
        return static_cast<const impl_type*>(this)->get_type();
    }

    int64_t execute(context &ctx) override {
        *get_pointer(left_->execute(ctx)) = assigned_value(ctx);
        return 1;
    }

    const char* get_name() const {
        return static_cast<const impl_type*>(this)->get_name();
    }

    virtual void to_program(std::ostream &ostr) const override {
        left_->to_program(ostr);
        ostr << " " << get_name() << " ";
        right_->to_program(ostr);
        ostr << ";";
    }

    void dump(std::ostream &ostr) const override {
        ostr << get_name() << " (";
        left_->dump(ostr);
        ostr << " ";
        right_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        auto node = graph.insert_node(graphviz_formatter::assignment, std::move(get_name()));
        parent.connect(graphviz_formatter::default_edge, node);

        left_->dump_gv(graph, node);
        right_->dump_gv(graph, node);
    }

protected:
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
};

class assign_node final: public assign_operation<assign_node> {
public:
    using assign_operation::assign_operation;

    node_type get_type() const noexcept override {
        return node_type::ASSIGN;
    }

    const char* get_name() const {
        return "=";
    }

    int64_t assigned_value(context &ctx) const {
        return get_value(right_->execute(ctx));
    }
};

class plus_assign_node final: public assign_operation<plus_assign_node> {
public:
    using assign_operation::assign_operation;

    node_type get_type() const noexcept override {
        return node_type::PLUSASSIGN;
    }

    const char* get_name() const {
        return "+=";
    }

    int64_t assigned_value(context &ctx) const {
        return std::plus<int64_t>()(get_value(left_->execute(ctx)), get_value(right_->execute(ctx)));
    }
};

class minus_assign_node final: public assign_operation<minus_assign_node> {
public:
    using assign_operation::assign_operation;

    node_type get_type() const noexcept override {
        return node_type::MINUSASSIGN;
    }

    const char* get_name() const {
        return "-=";
    }

    int64_t assigned_value(context &ctx) const {
        return std::minus<int64_t>()(get_value(left_->execute(ctx)), get_value(right_->execute(ctx)));
    }
};

class multiply_assign_node final: public assign_operation<multiply_assign_node> {
public:
    using assign_operation::assign_operation;

    node_type get_type() const noexcept override {
        return node_type::MULTIPLYASSIGN;
    }

    const char* get_name() const {
        return "*=";
    }

    int64_t assigned_value(context &ctx) const {
        return std::multiplies<int64_t>()(get_value(left_->execute(ctx)), get_value(right_->execute(ctx)));
    }
};

class divide_assign_node final: public assign_operation<divide_assign_node> {
public:
    using assign_operation::assign_operation;

    node_type get_type() const noexcept override {
        return node_type::DIVIDEASSIGN;
    }

    const char* get_name() const {
        return "/=";
    }

    int64_t assigned_value(context &ctx) const {
        return std::divides<int64_t>()(get_value(left_->execute(ctx)), get_value(right_->execute(ctx)));
    }
};


class negate_node final: public node {
public:
    explicit negate_node(std::unique_ptr<node> left):
        child_(std::move(left)) {}

    node_type get_type() const noexcept override {
        return node_type::NEGATE;
    }

    int64_t execute(context &ctx) override {
        return create_value(std::negate<int64_t>{}(get_value(child_->execute(ctx))));
    }

    std::string get_name() const {
        return "-";
    }

    virtual void to_program(std::ostream &ostr) const override {
        ostr << get_name();

        const node& ref = *child_;
        const std::type_info& ti = typeid(ref);
        if (ti == typeid(id_node) || ti == typeid(number_node)) {
            child_->to_program(ostr);
        }
        else {
            ostr << "(";
            child_->to_program(ostr);
            ostr << ")";
        }
    }

    void dump(std::ostream &ostr) const override {
        ostr << get_name() << " (";
        child_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        auto node = graph.insert_node(graphviz_formatter::negation, std::move(get_name()));
        parent.connect(graphviz_formatter::default_edge, node);

        child_->dump_gv(graph, node);
    }

private:
    std::unique_ptr<node> child_;
};

class divide_node;
//class scan_node;

template <typename impl_type, typename op>
class arithmetic_and_comparative_operator: public node {
public:
    explicit arithmetic_and_comparative_operator(std::unique_ptr<node> left,
                                                 std::unique_ptr<node> right):
        left_(std::move(left)), right_(std::move(right)) {}
    virtual ~arithmetic_and_comparative_operator() = default;

    node_type get_type() const noexcept override {
        return static_cast<const impl_type*>(this)->get_type();
    }

    int64_t execute(context &ctx) override {
        if constexpr (std::is_same_v<impl_type, divide_node>) {
            if (get_value(right_->execute(ctx)) == 0)
                throw std::runtime_error("divide by zero");
        }
        return create_value(op{}(get_value(left_->execute(ctx)),
                                 get_value(right_->execute(ctx))));
    }

    const char* get_name(bool for_graphviz = false) const {
        return static_cast<const impl_type*>(this)->get_name(for_graphviz);
    }

    virtual void to_program(std::ostream &ostr) const override {
        if(get_type() == node_type::PLUS || get_type() == node_type::MINUS) {
            left_->to_program(ostr);
            ostr << " " << get_name() << " ";
            if(right_->get_type() == node_type::PLUS || right_->get_type() == node_type::MINUS) {
                ostr << "(";
                right_->to_program(ostr);
                ostr << ")";
            }
            else {
                right_->to_program(ostr);
            }
        }
        else if(get_type() == node_type::MULTIPLY || get_type() == node_type::DIVIDE) {
            if(left_->get_type() == node_type::PLUS || left_->get_type() == node_type::MINUS) {
                ostr << "(";
                left_->to_program(ostr);
                ostr << ")";
            }
            else {
                left_->to_program(ostr);
            }
            ostr << " " << get_name() << " ";
            if(right_->get_type() == node_type::PLUS || right_->get_type() == node_type::MINUS ||
               right_->get_type() == node_type::MULTIPLY || right_->get_type() == node_type::DIVIDE) {
                ostr << "(";
                right_->to_program(ostr);
                ostr << ")";
            }
            else {
                right_->to_program(ostr);
            }
        }
        else {
            if(left_->get_type() == node_type::ID || left_->get_type() == node_type::NUMBER ||
               left_->get_type() == node_type::SCAN) {
                left_->to_program(ostr);
            }
            else {
                ostr << "(";
                left_->to_program(ostr);
                ostr << ")";
            }
            ostr << " " << get_name() << " ";
            if(right_->get_type() == node_type::ID || right_->get_type() == node_type::NUMBER ||
               right_->get_type() == node_type::SCAN) {
                right_->to_program(ostr);
            }
            else {
                ostr << "(";
                right_->to_program(ostr);
                ostr << ")";
            }
        }
    }

    void dump(std::ostream &ostr) const override {
        ostr << get_name() << " (";
        left_->dump(ostr);
        ostr << " ";
        right_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        auto node = graph.insert_node(graphviz_formatter::arithmetic_and_comparative,
                                      std::move(get_name(true)));
        parent.connect(graphviz_formatter::default_edge, node);

        left_->dump_gv(graph, node);
        right_->dump_gv(graph, node);
    }

protected:
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
};

class plus_node final: public arithmetic_and_comparative_operator<plus_node, std::plus<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::PLUS;
    }

    const char* get_name([[maybe_unused]] bool for_graphviz) const {
        return "+";
    }
};

class minus_node final: public arithmetic_and_comparative_operator<minus_node, std::minus<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::MINUS;
    }

    const char* get_name([[maybe_unused]] bool for_graphviz) const {
        return "-";
    }
};

class multiply_node final: public arithmetic_and_comparative_operator<multiply_node, std::multiplies<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::MULTIPLY;
    }

    const char* get_name([[maybe_unused]] bool for_graphviz) const {
        return "*";
    }
};

class divide_node final: public arithmetic_and_comparative_operator<divide_node, std::divides<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::DIVIDE;
    }

    const char* get_name([[maybe_unused]] bool for_graphviz) const {
        return "/";
    }
};

class equal_node final: public arithmetic_and_comparative_operator<equal_node, std::equal_to<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::EQUAL;
    }

    const char* get_name([[maybe_unused]] bool for_graphviz) const {
        return "==";
    }
};

class less_node final: public arithmetic_and_comparative_operator<less_node, std::less<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::LESS;
    }

    const char* get_name(bool for_graphviz) const {
        if(!for_graphviz) {
            return "<";
        }
        return "&lt;";
    }
};

class bigger_node final: public arithmetic_and_comparative_operator<bigger_node, std::greater<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::BIGGER;
    }

    const char* get_name(bool for_graphviz) const {
        if(!for_graphviz) {
            return ">";
        }
        return "&gt;";
    }
};

class less_or_equal_node final: public arithmetic_and_comparative_operator<less_or_equal_node,
                                                                           std::less_equal<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::LESSEQUAL;
    }

    const char* get_name(bool for_graphviz) const {
        if(!for_graphviz) {
            return "<=";
        }
        return "&le;";
    }
};

class bigger_or_equal_node final: public arithmetic_and_comparative_operator<bigger_or_equal_node,
                                                                             std::greater_equal<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    node_type get_type() const noexcept override {
        return node_type::BIGGEREQUAL;
    }

    const char* get_name(bool for_graphviz) const {
        if(!for_graphviz) {
            return ">=";
        }
        return "&ge;";
    }
};


template<bool is_loop>
class conditional_operation_node final: public node {
public:
    explicit conditional_operation_node(std::unique_ptr<node> condition,
                                        std::vector<std::unique_ptr<node>> scope):
        condition_(std::move(condition)), scope_(std::move(scope)) {}

    node_type get_type() const noexcept override {
        if constexpr (is_loop) {
            return node_type::WHILE;
        }
        return node_type::IF;
    }

    int64_t execute(context &ctx) override {
        while (get_value(condition_->execute(ctx)) != 0) {
            for (const auto& i: scope_) {
                i->execute(ctx);
            }
            if constexpr (!is_loop) {
                break;
            }
        }
        return 1;
    }

    std::string get_name() const {
        if constexpr (is_loop) {
            return "while";
        }
        return "if";
    }

    virtual void to_program(std::ostream &ostr) const override {
        ostr << get_name() << "(";
        condition_->to_program(ostr);
        ostr << ")\n{\n";
        for (const auto& i: scope_) {
            i->to_program(ostr);
            ostr << "\n";
        }
        ostr << "}\n";
    }

    void dump(std::ostream &ostr) const override {
        ostr << get_name() << " ((";
        condition_->dump(ostr);
        ostr << ")";
        for (const auto& i: scope_) {
            ostr << " (";
            i->dump(ostr);
            ostr << ")";
        }
        ostr << ")";
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        std::string label = get_name();
        auto node = graph.insert_node(graphviz_formatter::conditional, label);
        parent.connect(graphviz_formatter::default_edge, node);

        condition_->dump_gv(graph, node);
        for (const auto& i: scope_) {
            i->dump_gv(graph, node);
        }
    }

private:
    std::unique_ptr<node> condition_;
    std::vector<std::unique_ptr<node>> scope_;
};

using if_node    = conditional_operation_node<false>;
using while_node = conditional_operation_node<true>;


class scan_node final: public node {
public:
    node_type get_type() const noexcept override {
        return node_type::SCAN;
    }

    int64_t execute([[maybe_unused]] context &ctx) override {
        int64_t value;
        std::cout << "Input: ";
        std::cin >> value;
        return create_value(value);
    }

    virtual void to_program(std::ostream &ostr) const override {
        ostr << "?";
    }

    void dump(std::ostream &ostr) const override {
        ostr << "scan";
    }

    void dump_gv(graphviz &graph, node_proxy& parent) const override {
        auto node = graph.insert_node(graphviz_formatter::scan, "scan");
        parent.connect(graphviz_formatter::default_edge, node);
    }
};

} // end namespace paracl