#include "paracl/lexer/lexer.h"
#include "paracl/text/file.h"

#include <iostream>


int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]\n";
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
    std::string text = paracl::read_file(filename);

    std::vector<paracl::token> tokens = paracl::tokenize(text);
    paracl::print_tokens(tokens);
}

