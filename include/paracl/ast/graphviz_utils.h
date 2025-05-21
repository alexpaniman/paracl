#pragma once

#include "paracl/graphviz/graphviz.h"

namespace paracl {

namespace graphviz_formatter {

static inline graphviz_formatting number                     = {graphviz_color::ANI_BLUISH,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};
static inline graphviz_formatting id                         = {graphviz_color::ANI_PALE_BROWN,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};
static inline graphviz_formatting function                   = {graphviz_color::ANI_PEACH,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};
static inline graphviz_formatting assignment                 = {graphviz_color::ANI_PALE_SAND,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};
static inline graphviz_formatting negation                   = {graphviz_color::ANI_QUARTZ,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};
static inline graphviz_formatting arithmetic_and_comparative = {graphviz_color::ANI_LIGHT_GREY,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};
static inline graphviz_formatting conditional                = {graphviz_color::ANI_BROWN,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};
static inline graphviz_formatting scan                       = {graphviz_color::ANI_DUSTY_BLUE,
                                                                graphviz_style::FILLED,
                                                                graphviz_shape::RECORD};

static inline graphviz_formatting default_edge = {};

}  // end namespace graphviz_formatter

}  // end namespace paracl