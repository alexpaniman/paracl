#include "paracl/lexer/lexer.h"
#include "paracl/text/display.h"
#include "paracl/text/file.h"

#include <iostream>
#include <sstream>


int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]\n";
        return EXIT_FAILURE;
    }

    paracl::file source{argv[1], paracl::read_file(argv[1])};

    std::vector<paracl::token> tokens = paracl::tokenize(source.text);

    std::vector<paracl::rng> rngs;
    for (size_t i = 0; i < tokens.size() -3; ++ i) {
        std::stringstream ss;
        ss << "printing token #" << i;

        std::string description = paracl::describe_token(tokens[i]);

        paracl::rng r { tokens[i], description };
        source.message(ss.str(), {
            //r,
            {tokens[i], tokens[i+3], "note"},
            {tokens[i+1], tokens[i+2], "note"},
            {tokens[i+2], tokens[i+3], "note"},
            tokens[i]
        });

        rngs.push_back(r);

        // std::cout << "\n";
    }

    source.message("printing all tokens", rngs);

    // using namespace paracl;

    // print_range(source.text, {
    //     { { { 34, 3, 10 }, { 129, 10, 10 } },
    //     "note: this is it"
    //     },

    //     { { { 29, 3, 4 }, { 134, 10, 14 } },
    //     "note: this is it"
    //     },

    //     { { { 30, 3, 4 }, { 32, 3, 6 } },
    //     "note: this is it"
    //     },
    // });

    // for (uint32_t i = 0; i < tokens.size() - 2; ++ i) {

    // source.message("hi there", {
    //     { tokens[0], tokens[1], "this is a message" },
    //     // { tokens[3] },
    //     { tokens[5], "this is also a message" },
    //     // { tokens[16] },
    //     { tokens[6], "and yet another message for you" },
    // });

    // std::cout << "\n";
    // }
}

