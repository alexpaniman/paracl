#pragma once

#include "paracl/graphviz/attributes.h"

#include <list>
#include <string>
#include <iostream>


namespace paracl {

class graphviz final {
public:
    class node {
    public:
        graphviz_style style_;
        graphviz_color color_;
        graphviz_node_shape shape_;

        size_t index_;
        std::string label_;

        explicit node(size_t index, const std::string& label) noexcept:
            style_(graphviz_style::STYLE_FILLED),
            color_(graphviz_color::GRAPHVIZ_WHITE),
            shape_(graphviz_node_shape::SHAPE_RECORD),
            index_(index), label_(label) {}
        explicit node(graphviz_style style, graphviz_color color, graphviz_node_shape shape,
                      size_t index, const std::string& label) noexcept: 
            style_(style), color_(color), shape_(shape), index_(index), label_(label) {}
    };

    class edge {
    public:
        graphviz_style style_;
        graphviz_color color_;

        size_t from_;
        size_t to_;
        std::string label_;

        explicit edge(size_t from, size_t to, const std::string& label) noexcept:
            style_(graphviz_style::STYLE_FILLED), color_(graphviz_color::GRAPHVIZ_BLACK), 
            from_(from), to_(to), label_(label) {}
        explicit edge(graphviz_style style, graphviz_color color, 
                      size_t from, size_t to, const std::string& label) noexcept:
            style_(style), color_(color), from_(from), to_(to), label_(label) {}
    };

private:
    std::list<node> nodes_;
    std::list<edge> edges_;

public:
    template <typename node_type, auto create_node>
    void insert_node(node_type type, size_t index, const std::string& label) {
        static_assert(std::is_same_v<decltype(create_node(type, index, label)), node>);
        nodes_.push_back(create_node(type, index, label));
    }

    template <typename edge_type, auto create_edge>
    void insert_edge(edge_type type, size_t from, size_t to, const std::string& label) {
        static_assert(std::is_same_v<decltype(create_edge(type, from, to, label)), edge>);
        edges_.push_back(create_edge(type, from, to, label));  
    }

    void print(std::ostream &ostr = std::cout) {
        ostr << "digraph structs {\n";

        for(auto node: nodes_) {
            //надо будет сделать проверку find в каждом hash
            ostr << "    node" << node.index_ 
                 << "[shape = " << graphviz_node_shapes.at(node.shape_)
                 << ", label = \"{" << node.label_
                 << "}\", style = " << graphviz_styles.at(node.style_)
                 << ", fillcolor = \" " << graphviz_colors.at(node.color_) << "\"];\n";
        }
        for(auto edge: edges_) {
            //надо будет сделать проверку find в каждом hash
            ostr << "    node" << edge.from_ 
                 << "->node" << edge.to_
                 << " [style = " << graphviz_styles.at(edge.style_)
                 << ", fillcolor = \" " << graphviz_colors.at(edge.color_) << "\"];\n";
        }

        ostr << "}";
    }
};

} // end namespace paracl