#pragma once

#include <string>


namespace paracl {

enum graphviz_color {
    RED,
    BLUE,
    GREEN,
    BLACK,
    YELLOW,
    ORANGE,
    PINK,
    PURPLE,
    BROWN,
    WHITE,
    ANI_BROWN,
    ANI_PALE_SAND,
    ANI_PEACH,
    ANI_LIGHT_GREY,
    ANI_QUARTZ,
    ANI_PALE_BROWN,
    ANI_BLUISH,
    ANI_DUSTY_BLUE,
    COLOR_COUNT
};
extern const char* color_codes[COLOR_COUNT];

enum graphviz_style {
    FILLED,
    ROUNDED,
    DASHED,
    DIAGONALS,
    INVIS,
    BOLD,
    DOTTED,
    SOLID,
    STYLE_COUNT
};
extern const char* style_codes[STYLE_COUNT];

enum graphviz_shape {
    RECORD,
    BOX,
    POLYGON,
    ELLIPSE,
    OVAL,
    CIRCLE,
    POINT,
    DOUBLECIRCLE,
    DOUBLEOCTAGON,
    TRIPLEOCTAGON,
    INVTRIANGLE,
    INVTRAPEZIUM,
    INVHOUSE,
    EGG,
    TRIANGLE,
    PLAINTEXT,
    PLAIN,
    DIAMOND,
    TRAPEZIUM,
    PARALLELOGRAM,
    HOUSE,
    PENTAGON,
    HEXAGON,
    SEPTAGON,
    OCTAGON,
    SHAPE_COUNT
};
extern const char* shape_codes[SHAPE_COUNT];

class graphviz_formatting {
public:
    graphviz_formatting(graphviz_color color = graphviz_color::BLACK,
                        graphviz_style style = graphviz_style::FILLED,
                        graphviz_shape shape = graphviz_shape::RECORD):
    color_(color_codes[color]),
    style_(style_codes[style]),
    shape_(shape_codes[shape]) {}

public:
    std::string color_;
    std::string style_;
    std::string shape_;
};

} // end namespace paracl