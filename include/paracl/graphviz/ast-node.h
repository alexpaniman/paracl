#pragma once

#include "paracl/graphviz/graphviz.h"


namespace paracl {

enum class ast_node_type {
    function,
    id,
    number,
    assignment,
    negation,
    arithmetic_and_comparative,
    conditional,
    scan
};

class ast_node final {
public:
    std::string id_;
    ast_node_type type_;
    std::string label_;
};

graphviz::node ast_style(const ast_node& ast_node);

} // end namespace paracl