#include "paracl/graphviz/graphviz.h"


namespace paracl {

node_proxy graphviz::insert_node(graphviz_formatting format, const std::string& label) {
    nodes_.emplace_back(label, format);
    return node_proxy(*this, nodes_.size() - 1);
}

void graphviz::insert_edge(graphviz_formatting format, size_t from, size_t to, 
                                 const std::string& label) {
    edges_.emplace_back(from, to, label, format);
}

std::vector<graphviz::node>& graphviz::get_nodes() {
    return nodes_;
}

std::vector<graphviz::edge>& graphviz::get_edges() {
    return edges_;
}

void graphviz::print(std::ostream &ostr) {
    ostr << "digraph structs {\n";

    for(size_t i = 0; i < nodes_.size(); ++i) {
        auto node = nodes_[i];
        ostr << "    node" << i 
             << "[shape = " << shape_codes[node.formatting_.shape_]
             << ", fillcolor = \"" << color_codes[node.formatting_.color_]
             << "\", style = " << style_codes[node.formatting_.style_]
             << ", label = \"{" << node.label_ << "}\"];\n";
    }
    for(auto edge: edges_) {
        ostr << "    node" << edge.from_ 
             << "->node" << edge.to_
             << "[style = " << style_codes[edge.formatting_.style_]
             << ", fillcolor = \"" << color_codes[edge.formatting_.color_ ]<< "\"];\n";
    }

    ostr << "}";
}

} // end namespace paracl