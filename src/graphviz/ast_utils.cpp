#include "paracl/graphviz/ast_utils.h"


namespace paracl {

graphviz::node create_ast_node(ast_node_type type, size_t index, const std::string& label) {
    switch (type) {
        case ast_node_type::function: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_RED,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
        case ast_node_type::id: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_PINK,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
        case ast_node_type::number: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_ORANGE,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
        case ast_node_type::assignment: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_BLUE,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
        case ast_node_type::negation: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_GREEN,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
        case ast_node_type::arithmetic_and_comparative: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_PURPLE,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
        case ast_node_type::conditional: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_YELLOW,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
        case ast_node_type::scan: {
            graphviz::node node{graphviz_style::STYLE_FILLED,
                                graphviz_color::GRAPHVIZ_RED,
                                graphviz_node_shape::SHAPE_RECORD, index, label};
            return node;
        }
    }
}

graphviz::edge create_ast_edge(ast_edge_type type, size_t from, size_t to, const std::string& label) {
    switch (type) {
        case ast_edge_type::default_edge: {
            graphviz::edge edge{from, to, label};
            return edge;
        }
    }
}

} // end namespace paracl