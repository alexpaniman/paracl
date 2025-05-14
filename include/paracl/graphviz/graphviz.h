#pragma once

#include "paracl/graphviz/attributes.h"

#include <vector>
#include <string>
#include <iostream>


namespace paracl {

class node_proxy;
class edge_proxy;

class graphviz final {
public:
    node_proxy insert_node(graphviz_formatting format, const std::string& label);
    edge_proxy insert_edge(graphviz_formatting format, size_t from, size_t to, const std::string& label);

    void print(std::ostream &ostr = std::cout);

public:
    class node {
    public:
        graphviz_formatting formatting_;

        std::string label_;

        explicit node(const std::string& label) noexcept:
            formatting_(), label_(label) {}
        explicit node(graphviz_formatting formatting,
                      const std::string& label) noexcept: 
            formatting_(formatting), label_(label) {}
    };

    class edge {
    public:
        graphviz_formatting formatting_;

        size_t from_;
        size_t to_;
        std::string label_;

        explicit edge(size_t from, size_t to, const std::string& label) noexcept:
            formatting_(), from_(from), to_(to), label_(label) {}
        explicit edge(graphviz_formatting formatting, 
                      size_t from, size_t to, const std::string& label) noexcept:
            formatting_(formatting), from_(from), to_(to), label_(label) {}
    };

private:
    std::vector<node> nodes_;
    std::vector<edge> edges_;

public:
    std::vector<node>& get_nodes();
    std::vector<edge>& get_edges();
};

class edge_proxy {
public:
    edge_proxy(graphviz* graph, size_t index): graph_(graph), index_(index) {}

    graphviz::edge& get_edge() {
        return graph_->get_edges()[index_];
    }

private:
    graphviz* graph_;
    size_t index_;
};


class node_proxy {
public:
    node_proxy(graphviz* graph, size_t index): graph_(graph), index_(index) {}

    void connect(graphviz_formatting format, node_proxy other) {
        graph_->insert_edge(format, index_, other.index_, "");
    }

    graphviz::node& get_node() {
        return graph_->get_nodes()[index_];
    }

private:
    graphviz* graph_;
    size_t index_;
};

} // end namespace paracl