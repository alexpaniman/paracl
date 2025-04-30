#pragma once

#include "paracl/parser/parser.h"

#include <memory>
#include <unordered_map>


namespace paracl {

class ast {
public:
    ast(std::vector<token> tokens) {
        parser ast_parser{tokens, context_};
        scope_ = ast_parser.parse();
    }

    void run() {
        for (const auto& i : scope_) {
            i->execute(context_);
        }
    }

    void dump(std::ostream &ostr= std::cout) const {
        ostr << "main( ";
        for (const auto& i : scope_) {
            i->dump(ostr);
            ostr << " ";
        }
        ostr << ")";
    }

    void dump_gv(std::ostream &ostr = std::cout) const {
        graphviz graph{};

        size_t node_id = reinterpret_cast<size_t>(this);
        graph.insert_node<create_ast_node, ast_node_type>(ast_node_type::conditional, node_id, "program");
        for (const auto& i: scope_) {
            size_t child_id = reinterpret_cast<size_t>(i.get());
            graph.insert_edge<create_ast_edge, ast_edge_type>(ast_edge_type::default_edge, node_id, child_id, "");
            i->dump_gv(graph);
        }

        graph.print(ostr);
    }

private:
    std::vector<std::unique_ptr<node>> scope_{};
    context context_{};
};

} // end namespace paracl