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
const std::unordered_map<graphviz_color, std::string> graphviz_colors = {
    {GRAPHVIZ_RED,    "#ED7272FF"},
    {GRAPHVIZ_BLUE,   "#9ACEEB"},
    {GRAPHVIZ_GREEN,  "#94F097"},
    {GRAPHVIZ_BLACK,  "#000000"},
    {GRAPHVIZ_YELLOW, "#F8EDA2"},
    {GRAPHVIZ_ORANGE, "#FFB18DFF"},
    {GRAPHVIZ_PINK,   "#EAA9D6"},
    {GRAPHVIZ_PURPLE, "#D08DFFFF"},
    {GRAPHVIZ_BROWN,  "#857159FF"},
    {GRAPHVIZ_WHITE,  "#FFFFFFFF"}
};

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
const std::unordered_map<graphviz_style, std::string> graphviz_styles = {
    {STYLE_FILLED,    "filled"},
    {STYLE_ROUNDED,   "rounded"},
    {STYLE_DASHED,    "dashed"},
    {STYLE_DIAGONALS, "diagonals"},
    {STYLE_INVIS,     "invis"},
    {STYLE_BOLD,      "bold"},
    {STYLE_DOTTED,    "dotted"},
    {STYLE_SOLID,     "solid"}
};

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
const std::unordered_map<graphviz_node_shape, std::string> graphviz_node_shapes = {
    {SHAPE_RECORD,        "record"},
    {SHAPE_BOX,           "box"},
    {SHAPE_POLYGON,       "polygon"},
    {SHAPE_ELLIPSE,       "ellipse"},
    {SHAPE_OVAL,          "oval"},
    {SHAPE_CIRCLE,        "circle"},
    {SHAPE_POINT,         "point"},
    {SHAPE_EGG,           "egg"},
    {SHAPE_TRIANGLE,      "triangle"},
    {SHAPE_PLAINTEXT,     "plaintext"},
    {SHAPE_PLAIN,         "plain"},
    {SHAPE_DIAMOND,       "diamond"},
    {SHAPE_TRAPEZIUM,     "trapezium"},
    {SHAPE_PARALLELOGRAM, "parallelogram"},
    {SHAPE_HOUSE,         "house"},
    {SHAPE_PENTAGON,      "pentagon"},
    {SHAPE_HEXAGON,       "hexagon"},
    {SHAPE_SEPTAGON,      "septagon"},
    {SHAPE_OCTAGON,       "octagon"},
    {SHAPE_DOUBLECIRCLE,  "doublecircle"},
    {SHAPE_DOUBLEOCTAGON, "doubleoctagon"},
    {SHAPE_TRIPLEOCTAGON, "tripleoctagon"},
    {SHAPE_INVTRIANGLE,   "invtriangle"},
    {SHAPE_INVTRAPEZIUM,  "invtrapezium"},
    {SHAPE_INVHOUSE,      "invhouse"}
};

} // end namespace paracl