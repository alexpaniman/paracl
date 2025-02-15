#include "paracl/lexer/lexer.h"
#include "paracl/text/ansi.h"
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

    for (uint32_t i = 0; i < tokens.size() - 2; ++ i) {

        paracl::display_tokens(source, paracl::message_type::NOTE, "printing an annotated range", {
            { tokens[i],  paracl::CYAN + paracl::BOLD + "notes:" + paracl::RESET + " #i" },
            { tokens[i+1],  paracl::CYAN + paracl::BOLD + "notes:" + paracl::RESET + " #i+1" },
            { tokens[i+2],  paracl::CYAN + paracl::BOLD + "notes:" + paracl::RESET + " #i+2" },
            { tokens[5],  "" },
            { tokens[15], "" },
            { tokens[4],  paracl::CYAN + paracl::BOLD + "notes:" + paracl::RESET + " #i" },
        });

        std::cout << "\n";
    }
}

