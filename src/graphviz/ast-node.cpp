#include "paracl/graphviz/ast-node.h"
#include "paracl/graphviz/attributes.h"


namespace paracl {

graphviz::node ast_style(const ast_node& ast_node) {
    graphviz::node node;

    switch (ast_node.type_) {
        case ast_node_type::function:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_RED;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
        case ast_node_type::id:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_PINK;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
        case ast_node_type::number:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_ORANGE;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
        case ast_node_type::assignment:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_BLUE;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
        case ast_node_type::negation:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_GREEN;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
        case ast_node_type::arithmetic_and_comparative:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_PURPLE;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
        case ast_node_type::conditional:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_YELLOW;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
        case ast_node_type::scan:
            node.shape_ = graphviz_node_shape::SHAPE_RECORD;
            node.color_ = graphviz_color::GRAPHVIZ_RED;
            node.style_ = graphviz_style::STYLE_FILLED;
            break;
    }
    return node;
}

} // end namespace paracl