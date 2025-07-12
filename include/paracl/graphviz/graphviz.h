#pragma once

#include "paracl/graphviz/attributes.h"

#include <vector>
#include <string>
#include <iostream>


namespace paracl {

class node_proxy;

class graphviz final {
public:
    node_proxy insert_node(graphviz_formatting format, const std::string& label);
    void insert_edge(graphviz_formatting format, size_t from, size_t to, const std::string& label);

    void print(std::ostream &ostr = std::cout);

public:
    class node {
    public:
        std::string label_;

        graphviz_formatting formatting_;

        explicit node(std::string label) noexcept:
            label_(std::move(label)), formatting_() {}
        explicit node(std::string label, graphviz_formatting formatting) noexcept:
            label_(std::move(label)), formatting_(formatting) {}
    };

    class edge {
    public:
        size_t from_;
        size_t to_;
        std::string label_;

        graphviz_formatting formatting_;

        explicit edge(size_t from, size_t to, std::string label) noexcept:
            from_(from), to_(to), label_(std::move(label)), formatting_() {}
        explicit edge(size_t from, size_t to, std::string label,
                      graphviz_formatting formatting) noexcept:
            from_(from), to_(to), label_(std::move(label)), formatting_(formatting) {}
    };

private:
    std::vector<node> nodes_;
    std::vector<edge> edges_;

public:
    std::vector<node>& get_nodes();
    std::vector<edge>& get_edges();
};

class node_proxy {
public:
    node_proxy(graphviz& graph, size_t index): graph_(graph), index_(index) {}

    void connect(graphviz_formatting format, node_proxy other) {
        graph_.insert_edge(format, index_, other.index_, "");
    }

    graphviz::node& get_node() {
        return graph_.get_nodes()[index_];
    }

private:
    graphviz& graph_;
    size_t index_;
};

} // end namespace paracl