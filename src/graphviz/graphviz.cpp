#include "paracl/graphviz/graphviz.h"


namespace paracl {

node_proxy graphviz::insert_node(graphviz_formatting format, const std::string& label) {
    nodes_.push_back(node(format, label));
    return node_proxy(this, nodes_.size() - 1);
}

edge_proxy graphviz::insert_edge(graphviz_formatting format, size_t from, size_t to, 
                                 const std::string& label) {
    edges_.push_back(edge(format, from, to, label));
    return edge_proxy(this, edges_.size() - 1);
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
             << "[shape = " << node.formatting_.shape_
             << ", fillcolor = \"" << node.formatting_.color_
             << "\", style = " << node.formatting_.style_
             << ", label = \"{" << node.label_ << "}\"];\n";
    }
    for(auto edge: edges_) {
        ostr << "    node" << edge.from_ 
             << "->node" << edge.to_
             << "[style = " << edge.formatting_.style_
             << ", fillcolor = \"" << edge.formatting_.color_ << "\"];\n";
    }

    ostr << "}";
}

} // end namespace paracl