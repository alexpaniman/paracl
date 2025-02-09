#pragma once

#include "paracl/ast/context.h"
#include "paracl/ast/bit_operations.h"

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <iomanip>


namespace paracl {

class node {
public:
    virtual int64_t execute(context &ctx) = 0;
    virtual void dump_gv(std::ostream &ostr) const = 0;
    virtual void dump(std::ostream &ostr) const = 0;
    virtual ~node() = default;
};

class number_node final: public node {
public:
    explicit number_node(int64_t value):
        value_(value) {}

    int64_t execute([[maybe_unused]] context &ctx) override {
        return create_value(value_);
    }

    void dump(std::ostream &ostr) const override {
        ostr << value_;
    }

    void dump_gv(std::ostream &ostr) const override {
        ostr << "    node" << this << "[shape = Mrecord, label = \"{" << value_
             << "}\", style = \"filled\", fillcolor = \"#EAA9D6\"];\n";
    }

private:
    int64_t value_;
};


class id_node final: public node {
public:
    explicit id_node(std::string name):
        name_(std::move(name)) {}

    int64_t execute(context &ctx) override {
        return create_pointer(ctx.get_variable(name_));
    }

    void dump(std::ostream &ostr) const override {
        ostr << name_;
    }

    void dump_gv(std::ostream &ostr) const override {
        ostr << "    node" << this << "[shape = Mrecord, label = \"{" << name_
             << "}\", style = \"filled\", fillcolor = \"#EAA9D6\"];\n";
    }

private:
    std::string name_;
};

class function_node final: public node {
public:
    explicit function_node(std::string name, std::vector<std::unique_ptr<node>> args):
        name_(std::move(name)), args_(std::move(args)) {}

    int64_t execute(context &ctx) override {
        if (name_ == "print") {
            for (const auto& i: args_) {
                std::cout << get_value(i->execute(ctx)) << " ";
            }
            std::cout << std::endl;
            return 1;
        }
        return 0; //остальные функции пока не реализованы
    }

    void dump(std::ostream &ostr) const override {
        ostr << name_ << "( ";
        for (const auto& i: args_) {
            i->dump(ostr);
            ostr << " ";
        }
        ostr << ")";
    }

    void dump_gv(std::ostream &ostr) const override {
        ostr << "    node" << this << "[shape=Mrecord, label=\"{" << name_
             << "}\", style=filled, fillcolor=\"#CC9CFF\"]\n";
        for (const auto& i: args_) {
            ostr << "    node" << this << "->node" << i.get()
                 << " [color = \"#293133\"]\n";
            i->dump_gv(ostr);
        }
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<node>> args_;
};

class assign_node final: public node {
public:
    explicit assign_node(std::unique_ptr<node> left, std::unique_ptr<node> right):
        left_(std::move(left)), right_(std::move(right)) {}

    int64_t execute(context &ctx) override {
        *get_pointer(left_->execute(ctx)) = get_value(right_->execute(ctx));
        return 1;
    }

    void dump(std::ostream &ostr) const override {
        ostr << "= (";
        left_->dump(ostr);
        ostr << " ";
        right_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(std::ostream &ostr) const override {
        ostr << "    node" << this << "[shape = Mrecord, label = \"{=}\", "
             << "style = \"filled\", fillcolor = \"#9ACEEB\"];\n";

        ostr << "    node" << this << "->node" << left_.get() << " [color = \"#293133\"];\n";
        left_->dump_gv(ostr);

        ostr << "    node" << this << "->node" << right_.get() << " [color = \"#293133\"];\n";
        right_->dump_gv(ostr);
    }

private:
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
};

class unary_operation: public node {
public:
    explicit unary_operation(std::unique_ptr<node> left):
        child_(std::move(left)) {}

protected:
    std::unique_ptr<node> child_;
};

template<typename op>
class unary_arithmetic_node final: public unary_operation {
public:
    using unary_operation::unary_operation;

    int64_t execute(context &ctx) override {
        std::cerr << get_value(child_->execute(ctx)) << std::endl;
        std::cerr << op{}(get_value(child_->execute(ctx))) << std::endl;
        return create_value(op{}(get_value(child_->execute(ctx))));
    }

    std::string to_string() const {
        if constexpr (std::is_same_v<op, std::negate<int64_t>>)
            return "-";
        return "?";
    }

    void dump(std::ostream &ostr) const override {
        ostr << to_string() << " (";
        child_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(std::ostream &ostr) const override {
        ostr << "    node" << this
             << "[shape=Mrecord, label=\"{" << to_string()
             << "}\", style=filled, fillcolor=\"#9ACEEB\"];\n";
        ostr << "    node" << this
             << "->node" << child_.get() << " [color=\"#293133\"];\n";
        child_->dump_gv(ostr);
    }
};

using negate_node = paracl::unary_arithmetic_node<std::negate<int64_t>>;

class binary_operation: public node {
public:
    explicit binary_operation(std::unique_ptr<node> left, std::unique_ptr<node> right):
        left_(std::move(left)), right_(std::move(right)) {}

protected:
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
};

enum class binary_node_type {
    arithmetic,
    assignment,
    comparation
};

template<typename op, unsigned int node_color, binary_node_type type>
class binary_node final: public binary_operation {
public:
    using binary_operation::binary_operation;

    int64_t execute(context &ctx) override {
        if constexpr ((type == binary_node_type::arithmetic || type == binary_node_type::assignment) &&
                      std::is_same_v<op, std::divides<int64_t>>) {
            if (get_value(right_->execute(ctx)) == 0)
                throw std::runtime_error("divide by zero");
        }

        if constexpr (type == binary_node_type::arithmetic || type == binary_node_type::comparation) {
            return create_value(op{}(get_value(left_->execute(ctx)),
                                     get_value(right_->execute(ctx))));
        }
        else if constexpr (type == binary_node_type::assignment) {
            *get_pointer(left_->execute(ctx)) = op{}(get_value(left_->execute(ctx)),
                                                      get_value(right_->execute(ctx)));
            return 1;
        }
    }

    std::string to_string() const {
        if constexpr (type == binary_node_type::arithmetic) {
            if constexpr (std::is_same_v<op, std::plus<int64_t>>)
                return "+";
            else if constexpr (std::is_same_v<op, std::minus<int64_t>>)
                return "-";
            else if constexpr (std::is_same_v<op, std::multiplies<int64_t>>)
                return "*";
            else if constexpr (std::is_same_v<op, std::divides<int64_t>>)
                return "/";
        }
        else if constexpr (type == binary_node_type::assignment) {
            if constexpr (std::is_same_v<op, std::plus<int64_t>>)
                return "+=";
            else if constexpr (std::is_same_v<op, std::minus<int64_t>>)
                return "-=";
            else if constexpr (std::is_same_v<op, std::multiplies<int64_t>>)
                return "*=";
            else if constexpr (std::is_same_v<op, std::divides<int64_t>>)
                return "/=";
        }
        else if constexpr (type == binary_node_type::comparation) {
            if constexpr (std::is_same_v<op, std::equal_to<int64_t>>)
                return "==";
            else if constexpr (std::is_same_v<op, std::less<int64_t>>)
                return "&lt;";
            else if constexpr (std::is_same_v<op, std::greater<int64_t>>)
                return "&gt;";
            else if constexpr (std::is_same_v<op, std::less_equal<int64_t>>)
                return "&le;";
            else if constexpr (std::is_same_v<op, std::greater_equal<int64_t>>)
                return "&ge;";
        }
        return "?";
    }

    void dump(std::ostream &ostr) const override {
        ostr << to_string() << " (";
        left_->dump(ostr);
        ostr << " ";
        right_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(std::ostream &ostr) const override {
        ostr << "    node" << this
             << "[shape=Mrecord, label=";
        if constexpr (type == binary_node_type::comparation)
            ostr << "<<B>" << to_string() << "</B>>";
        else
            ostr << "\"" << to_string() << "\"";
        ostr << ", style=filled, fillcolor=\"#" << std::hex << node_color << "\"];\n";
        ostr << "    node" << this
             << "->node" << left_.get() << " [color=\"#293133\"];\n";
        left_->dump_gv(ostr);
        ostr << "    node" << this
             << "->node" << right_.get() << " [color=\"#293133\"];\n";
        right_->dump_gv(ostr);
    }
};

using plus_node             = binary_node<std::plus<int64_t>,         0x9ACEEB, binary_node_type::arithmetic>;
using minus_node            = binary_node<std::minus<int64_t>,        0x9ACEEB, binary_node_type::arithmetic>;
using multiply_node         = binary_node<std::multiplies<int64_t>,   0x9ACEEB, binary_node_type::arithmetic>;
using divide_node           = binary_node<std::divides<int64_t>,      0x9ACEEB, binary_node_type::arithmetic>;

using plus_assign_node     = binary_node<std::plus<int64_t>,          0x9ACEEB, binary_node_type::assignment>;
using minus_assign_node    = binary_node<std::minus<int64_t>,         0x9ACEEB, binary_node_type::assignment>;
using multiply_assign_node = binary_node<std::multiplies<int64_t>,    0x9ACEEB, binary_node_type::assignment>;
using divide_assign_node   = binary_node<std::divides<int64_t>,       0x9ACEEB, binary_node_type::assignment>;

using equal_node           = binary_node<std::equal_to<int64_t>,      0xC5E384, binary_node_type::comparation>;
using less_node            = binary_node<std::less<int64_t>,          0xC5E384, binary_node_type::comparation>;
using bigger_node          = binary_node<std::greater<int64_t>,       0xC5E384, binary_node_type::comparation>;
using less_or_equal_node   = binary_node<std::less_equal<int64_t>,    0xC5E384, binary_node_type::comparation>;
using bigger_or_equal_node = binary_node<std::greater_equal<int64_t>, 0xC5E384, binary_node_type::comparation>;


template<bool is_loop>
class conditional_operation_node: public node {
public:
    explicit conditional_operation_node(std::unique_ptr<node> condition,
                                        std::vector<std::unique_ptr<node>> scope):
        condition_(std::move(condition)), scope_(std::move(scope)) {}

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

    std::string to_string() const {
        return is_loop ? "while" : "if";
    }

    void dump(std::ostream &ostr) const override {
        ostr << to_string() << " ((";
        condition_->dump(ostr);
        ostr << ")";
        for (const auto& i: scope_) {
            ostr << " (";
            i->dump(ostr);
            ostr << ")";
        }
        ostr << ")";
    }

    void dump_gv(std::ostream &ostr) const override {
        std::string label = is_loop ? "while" : "if";
        ostr << "    node" << this << "[shape=Mrecord, label=\"{" << label
             << "}\", style=filled, fillcolor=\"#F8EDA2\"]\n";
        ostr << "    node" << this << "->node" << condition_.get()
             << " [color = \"#FF2B2B\"]\n";
        condition_->dump_gv(ostr);
        for (const auto& i: scope_) {
            ostr << "    node" << this << "->node" << i.get()
                 << " [color = \"#293133\"]\n";
            i->dump_gv(ostr);
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
    int64_t execute([[maybe_unused]] context &ctx) override {
        int64_t value;
        std::cout << "Input: ";
        std::cin >> value;
        return create_value(value);
    }

    void dump(std::ostream &ostr) const override {
        ostr << "scan";
    }

    void dump_gv(std::ostream &ostr) const override {
        ostr << "    node" << this << "[shape = Mrecord, label = \"{scanf}\", "
             << "style = \"filled\", fillcolor = \"#EAA9D6\"];\n";
    }
};

} // end namespace paracl