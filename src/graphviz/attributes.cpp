#include "paracl/graphviz/attributes.h"


namespace paracl {

const char* color_codes[COLOR_COUNT] = {
    "#ED7272FF", "#9ACEEB",   "#94F097",   "#000000",   "#BE7757", "#FFB18DFF",
    "#EAA9D6",   "#D08DFFFF", "#857159FF", "#FFFFFFFF", "#B08A71", "#DBBFAA",
    "#DEA875",   "#D1D0CB",   "#A2988F",   "#E1B5B4",   "#8CBED7", "#B1E0E9"};

const char* style_codes[STYLE_COUNT] = {"filled",    "rounded", "dashed",
                                        "diagonals", "invis",   "bold",
                                        "dotted",    "solid"};

const char* shape_codes[SHAPE_COUNT] = {
    "record",        "box",           "polygon",     "ellipse",
    "oval",          "circle",        "point",       "doublecircle",
    "doubleoctagon", "tripleoctagon", "invtriangle", "invtrapezium",
    "invhouse",      "egg",           "triangle",    "plaintext",
    "plain",         "diamond",       "trapezium",   "parallelogram",
    "house",         "pentagon",      "hexagon",     "septagon",
    "octagon"};

} // end namespace paracl