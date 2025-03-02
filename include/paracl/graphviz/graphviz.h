#pragma once

#include "paracl/graphviz/attributes.h"

#include <list>
#include <string>


namespace paracl {

class graphviz final {
public:
    class node {
    public:
        graphviz_style style_;
        graphviz_color color_;
        graphviz_node_shape shape_;

        std::string label_;
    };

    class edge {
    public:
        int from_;
        int to_;

        graphviz_color color_;
        graphviz_style style_;

        std::string label_;
    };

private:
    std::list<node> nodes_;
    std::list<edge> edges_;

public:
    template <typename node_type>
    node create_node(node_type type, const std::string& label);
    template <typename node_type>
    void add_node(node_type type, const std::string& label);

    void add_edge(const std::string& from, const std::string& to, 
                  const std::string& label = "");
};

} // end namespace paracl