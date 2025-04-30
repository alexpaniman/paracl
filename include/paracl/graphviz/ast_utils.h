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

graphviz::node create_ast_node(ast_node_type type, size_t index, const std::string& label);

enum class ast_edge_type {
    default_edge
};

graphviz::edge create_ast_edge(ast_edge_type type, size_t from, size_t to, const std::string& label);

} // end namespace paracl