#pragma once

#include "paracl/ast/context.h"
#include "paracl/ast/marked_pointers.h"
#include "paracl/graphviz/ast_utils.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>


namespace paracl {

class node {
public:
    virtual int64_t execute(context &ctx) = 0;
    virtual void dump_gv(graphviz &graph) const = 0;
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

    void dump_gv(graphviz &graph) const override {
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::number, node_id, std::to_string(value_));
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

    void dump_gv(graphviz &graph) const override {
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::id, node_id, name_);
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

    void dump(std::ostream &ostr) const override {
        ostr << name_ << "( ";
        for (const auto& i: args_) {
            i->dump(ostr);
            ostr << " ";
        }
        ostr << ")";
    }

    void dump_gv(graphviz &graph) const override {
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::function, node_id, name_);

        for (const auto& i: args_) {
            size_t child_id = reinterpret_cast<size_t>(i.get());
            graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
            i->dump_gv(graph);
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

    const char* get_name() const {
        return static_cast<const impl_type*>(this)->get_name();
    }

    int64_t assigned_value(context &ctx) const {
        return static_cast<const impl_type*>(this)->assigned_value(ctx);
    }

    int64_t execute(context &ctx) override {
        *get_pointer(left_->execute(ctx)) = assigned_value(ctx);
        return 1;
    }

    void dump(std::ostream &ostr) const override {
        ostr << get_name() << " (";
        left_->dump(ostr);
        ostr << " ";
        right_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(graphviz &graph) const override {
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::assignment, node_id, get_name());

        size_t child_id = reinterpret_cast<size_t>(left_.get());
        graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
        left_->dump_gv(graph);

        child_id = reinterpret_cast<size_t>(right_.get());
        graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
        right_->dump_gv(graph);
    }

protected:
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
};

class assign_node final: public assign_operation<assign_node> {
public:
    using assign_operation::assign_operation;
    
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

    int64_t execute(context &ctx) override {
        return create_value(std::negate<int64_t>{}(get_value(child_->execute(ctx))));
    }

    std::string get_name() const {
        return "-";
    }

    void dump(std::ostream &ostr) const override {
        ostr << get_name() << " (";
        child_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(graphviz &graph) const override {
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::negation, node_id, get_name());

        size_t child_id = reinterpret_cast<size_t>(child_.get());
        graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
        child_->dump_gv(graph);
    }

private:
    std::unique_ptr<node> child_;
};


class divide_node;

template <typename impl_type, typename op>
class arithmetic_and_comparative_operator: public node {
public:
    explicit arithmetic_and_comparative_operator(std::unique_ptr<node> left, 
                                                 std::unique_ptr<node> right):
        left_(std::move(left)), right_(std::move(right)) {}
    virtual ~arithmetic_and_comparative_operator() = default;

    const char* get_name() const {
        return static_cast<const impl_type*>(this)->get_name();
    }

    int64_t execute(context &ctx) override {
        if constexpr (std::is_same_v<impl_type, divide_node>) {
            if (get_value(right_->execute(ctx)) == 0)
                throw std::runtime_error("divide by zero");
        }
        return create_value(op{}(get_value(left_->execute(ctx)),
                                 get_value(right_->execute(ctx))));
    }

    void dump(std::ostream &ostr) const override {
        ostr << get_name() << " (";
        left_->dump(ostr);
        ostr << " ";
        right_->dump(ostr);
        ostr << ")";
    }

    void dump_gv(graphviz &graph) const override {
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::arithmetic_and_comparative, node_id, get_name());

        size_t child_id = reinterpret_cast<size_t>(left_.get());
        graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
        left_->dump_gv(graph);

        child_id = reinterpret_cast<size_t>(right_.get());
        graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
        right_->dump_gv(graph);
    }

protected:
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
};

class plus_node final: public arithmetic_and_comparative_operator<plus_node, std::plus<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    const char* get_name() const {
        return "+";
    }
};

class minus_node final: public arithmetic_and_comparative_operator<minus_node, std::minus<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;
        
    const char* get_name() const {
        return "-";
    }
};

class multiply_node final: public arithmetic_and_comparative_operator<multiply_node, std::multiplies<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    const char* get_name() const {
        return "*";
    }
};

class divide_node final: public arithmetic_and_comparative_operator<divide_node, std::divides<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;
        
    const char* get_name() const {
        return "/";
    }
};

class equal_node final: public arithmetic_and_comparative_operator<equal_node, std::equal_to<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;
            
    const char* get_name() const {
        return "==";
    }
};

class less_node final: public arithmetic_and_comparative_operator<less_node, std::less<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;
                
    const char* get_name() const {
        return "&lt;";
    }
};

class bigger_node final: public arithmetic_and_comparative_operator<bigger_node, std::greater<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;

    const char* get_name() const {
        return "&gt;";
    }
};

class less_or_equal_node final: public arithmetic_and_comparative_operator<less_or_equal_node, 
                                                                           std::less_equal<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;
    
    const char* get_name() const {
        return "&le;";
    }
};

class bigger_or_equal_node final: public arithmetic_and_comparative_operator<bigger_or_equal_node, 
                                                                             std::greater_equal<int64_t>> {
public:
    using arithmetic_and_comparative_operator::arithmetic_and_comparative_operator;
        
    const char* get_name() const {
        return "&ge;";
    }
};


template<bool is_loop>
class conditional_operation_node final: public node {
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

    std::string get_name() const {
        return is_loop ? "while" : "if";
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

    void dump_gv(graphviz &graph) const override {
        std::string label = is_loop ? "while" : "if";
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::conditional, node_id, label);

        size_t child_id = reinterpret_cast<size_t>(condition_.get());
        graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
        condition_->dump_gv(graph);
        for (const auto& i: scope_) {
            child_id = reinterpret_cast<size_t>(i.get());
            graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
            i->dump_gv(graph);
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

    void dump_gv(graphviz &graph) const override {
        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::scan, node_id, "scan");
    }
};

} // end namespace paracl