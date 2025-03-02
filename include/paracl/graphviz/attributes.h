#pragma once

#include <unordered_map>
#include <string>


namespace paracl {

enum graphviz_color {
    GRAPHVIZ_RED,
    GRAPHVIZ_BLUE,
    GRAPHVIZ_GREEN,
    GRAPHVIZ_BLACK,
    GRAPHVIZ_YELLOW,
    GRAPHVIZ_ORANGE,
    GRAPHVIZ_PINK,
    GRAPHVIZ_PURPLE,
    GRAPHVIZ_BROWN,
    GRAPHVIZ_WHITE
};
const std::unordered_map<graphviz_color, std::string> graphviz_colors;

enum graphviz_style {
    STYLE_FILLED,
    STYLE_ROUNDED,
    STYLE_DASHED,
    STYLE_DIAGONALS,
    STYLE_INVIS,
    STYLE_BOLD,
    STYLE_DOTTED,
    STYLE_SOLID
};
const std::unordered_map<graphviz_style, std::string> graphviz_styles;

enum graphviz_node_shape {
    SHAPE_RECORD,
    SHAPE_BOX,
    SHAPE_POLYGON,
    SHAPE_ELLIPSE,
    SHAPE_OVAL,
    SHAPE_CIRCLE,
    SHAPE_POINT,
    SHAPE_DOUBLECIRCLE,
    SHAPE_DOUBLEOCTAGON,
    SHAPE_TRIPLEOCTAGON,
    SHAPE_INVTRIANGLE,
    SHAPE_INVTRAPEZIUM,
    SHAPE_INVHOUSE,
    SHAPE_EGG,
    SHAPE_TRIANGLE,
    SHAPE_PLAINTEXT,
    SHAPE_PLAIN,
    SHAPE_DIAMOND,
    SHAPE_TRAPEZIUM,
    SHAPE_PARALLELOGRAM,
    SHAPE_HOUSE,
    SHAPE_PENTAGON,
    SHAPE_HEXAGON,
    SHAPE_SEPTAGON,
    SHAPE_OCTAGON
};
const std::unordered_map<graphviz_node_shape, std::string> graphviz_node_shapes;

} // end namespace paracl