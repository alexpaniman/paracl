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
        ostr << "digraph structs {\n";
        ostr << "    node" << this << "[shape=Mrecord, label= \"{program}\", "
             << "style=filled, fillcolor=\"#F8EDA2\"]\n";
        for (const auto& i : scope_) {
            ostr << "    node" << this << "->node" << i.get()
                 << " [color = \"#293133\"]\n";
            i->dump_gv(ostr);
        }
        ostr << "}";
    }

private:
    std::vector<std::unique_ptr<node>> scope_{};
    context context_{};
};

} // end namespace paracl