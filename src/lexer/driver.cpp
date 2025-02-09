#include "paracl/lexer/lexer.h"
#include "paracl/text/display.h"
#include "paracl/text/file.h"

#include <iostream>


int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]\n";
        return EXIT_FAILURE;
    }

    paracl::file source{argv[1], paracl::read_file(argv[1])};

    std::vector<paracl::token> tokens = paracl::tokenize(source.text);
    paracl::print_tokens(tokens);

    for (uint32_t i = 0; i < tokens.size(); ++ i) {
        auto tok1 = tokens[i];
        std::vector<paracl::token> toks{tok1};
        paracl::display_tokens(source, paracl::message_type::NOTE, "printing random range:", toks);
    }
}

