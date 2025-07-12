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

    void to_program(std::ostream &ostr= std::cout) const {
        for (const auto& i : scope_) {
            i->to_program(ostr);
            ostr << "\n";
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

        auto node = graph.insert_node(graphviz_formatter::conditional, "program");
        for (const auto& i: scope_) {
            i->dump_gv(graph, node);
        }

        graph.print(ostr);
    }

private:
    std::vector<std::unique_ptr<node>> scope_{};
    context context_{};
};

} // end namespace paracl