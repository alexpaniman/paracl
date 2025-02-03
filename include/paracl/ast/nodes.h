#pragma once

#include "paracl/ast/context.h"
#include "paracl/ast/bit_operations.h"

#include <iostream>
#include <memory>
#include <vector>
#include <functional>


namespace paracl {

class node {
public:
    virtual uint64_t execute(context &ctx) = 0;
    virtual void dump_gv(std::ostream &ostr) const = 0;
    virtual void dump(std::ostream &ostr) const = 0;
    virtual ~node() = default;
};

class number_node final: public node {
public:
    explicit number_node(uint64_t value):
        value_(value) {}

    uint64_t execute([[maybe_unused]] context &ctx) override {
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
    uint64_t value_;
};


class id_node final: public node {
public:
    explicit id_node(std::string name):
        name_(std::move(name)) {}

    uint64_t execute(context &ctx) override {
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

    uint64_t execute(context &ctx) override {
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
             << "}\", style=filled, fillcolor=\"#B571FC\"]\n";
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

    uint64_t execute(context &ctx) override {
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

class binary_operation: public node {
public:
    explicit binary_operation(std::unique_ptr<node> left, std::unique_ptr<node> right):
        left_(std::move(left)), right_(std::move(right)) {}

protected:
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
};

template<typename op>
class binary_arithmetic_node final: public binary_operation {
public:
    using binary_operation::binary_operation;

    uint64_t execute(context &ctx) override {
        if constexpr (std::is_same_v<op, std::divides<uint64_t>>) {
            if (get_value(right_->execute(ctx)) == 0)
                throw std::runtime_error("divide by zero");
        }
        return create_value(op{}(get_value(left_->execute(ctx)),
                                 get_value(right_->execute(ctx))));
    }

    std::string to_string() const {
        if constexpr (std::is_same_v<op, std::plus<uint64_t>>)
            return "+";
        else if constexpr (std::is_same_v<op, std::minus<uint64_t>>)
            return "-";
        else if constexpr (std::is_same_v<op, std::multiplies<uint64_t>>)
            return "*";
        else if constexpr (std::is_same_v<op, std::divides<uint64_t>>)
            return "/";
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
             << "[shape=Mrecord, label=\"{" << to_string()
             << "}\", style=filled, fillcolor=\"#9ACEEB\"];\n";
        ostr << "    node" << this
             << "->node" << left_.get() << " [color=\"#293133\"];\n";
        left_->dump_gv(ostr);
        ostr << "    node" << this
             << "->node" << right_.get() << " [color=\"#293133\"];\n";
        right_->dump_gv(ostr);
    }
};

using plus_node     = paracl::binary_arithmetic_node<std::plus<uint64_t>>;
using minus_node    = paracl::binary_arithmetic_node<std::minus<uint64_t>>;
using multiply_node = paracl::binary_arithmetic_node<std::multiplies<uint64_t>>;
using divide_node   = paracl::binary_arithmetic_node<std::divides<uint64_t>>;


template<typename op>
class binary_assign_node final: public binary_operation {
public:
    using binary_operation::binary_operation;

    uint64_t execute(context &ctx) override {
        if constexpr (std::is_same_v<op, std::divides<uint64_t>>) {
            if (get_value(right_->execute(ctx)) == 0) {
                throw std::runtime_error("divide by zero");
            }
        }
        *get_pointer(left_->execute(ctx)) = op{}(get_value(right_->execute(ctx)), 
                                                 get_value(left_->execute(ctx)));
        return 1;
    }

    std::string to_string() const {
        if constexpr (std::is_same_v<op, std::plus<uint64_t>>)
            return "+=";
        else if constexpr (std::is_same_v<op, std::minus<uint64_t>>)
            return "-=";
        else if constexpr (std::is_same_v<op, std::multiplies<uint64_t>>)
            return "*=";
        else if constexpr (std::is_same_v<op, std::divides<uint64_t>>)
            return "/=";
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
             << "[shape=Mrecord, label=\"{" << to_string()
             << "}\", style=filled, fillcolor=\"#9ACEEB\"];\n";
        ostr << "    node" << this
             << "->node" << left_.get() << " [color=\"#293133\"];\n";
        left_->dump_gv(ostr);
        ostr << "    node" << this
             << "->node" << right_.get() << " [color=\"#293133\"];\n";
        right_->dump_gv(ostr);
    }
};

using plus_assign_node     = paracl::binary_assign_node<std::plus<uint64_t>>;
using minus_assign_node    = paracl::binary_assign_node<std::minus<uint64_t>>;
using multiply_assign_node = paracl::binary_assign_node<std::multiplies<uint64_t>>;
using divide_assign_node   = paracl::binary_assign_node<std::divides<uint64_t>>;


template<typename op>
class binary_comparation_node final: public binary_operation {
public:
    using binary_operation::binary_operation;

    uint64_t execute(context &ctx) override {
        return create_value(op{}(get_value(left_->execute(ctx)),
                                 get_value(right_->execute(ctx))));
    }

    std::string to_string() const {
        if constexpr (std::is_same_v<op, std::equal_to<uint64_t>>)
            return "==";
        else if constexpr (std::is_same_v<op, std::less<uint64_t>>)
            return "&lt;";
        else if constexpr (std::is_same_v<op, std::greater<uint64_t>>)
            return "&gt;";
        else if constexpr (std::is_same_v<op, std::less_equal<uint64_t>>)
            return "&le;";
        else if constexpr (std::is_same_v<op, std::greater_equal<uint64_t>>)
            return "&ge;";
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
             << "[shape=Mrecord, label=<<B>" << to_string()
             << "</B>>, style=filled, fillcolor=\"#C5E384\"];\n";
        ostr << "    node" << this
             << "->node" << left_.get() << " [color=\"#293133\"];\n";
        left_->dump_gv(ostr);
        ostr << "    node" << this
             << "->node" << right_.get() << " [color=\"#293133\"];\n";
        right_->dump_gv(ostr);
    }
};

using equal_node           = paracl::binary_comparation_node<std::equal_to<uint64_t>>;
using less_node            = paracl::binary_comparation_node<std::less<uint64_t>>;
using bigger_node          = paracl::binary_comparation_node<std::greater<uint64_t>>;
using less_or_equal_node   = paracl::binary_comparation_node<std::less_equal<uint64_t>>;
using bigger_or_equal_node = paracl::binary_comparation_node<std::greater_equal<uint64_t>>;


template<bool is_loop>
class conditional_operation_node: public node {
public:
    explicit conditional_operation_node(std::unique_ptr<node> condition,
                                        std::vector<std::unique_ptr<node>> scope):
        condition_(std::move(condition)), scope_(std::move(scope)) {}

    uint64_t execute(context &ctx) override {
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

using if_node    = paracl::conditional_operation_node<false>;
using while_node = paracl::conditional_operation_node<true>;


class Scan final: public node {
public:
    uint64_t execute([[maybe_unused]] context &ctx) override {
        uint64_t value;
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