#include "paracl/text/file.h"

#include <sstream>
#include <fstream>


namespace paracl {

std::string read_file(const std::string &filename) {
    std::ifstream read_text_stream{filename};

    std::stringstream saved_saved_stream;
    saved_saved_stream << read_text_stream.rdbuf();

    return saved_saved_stream.str();
}

} // end namespace paracl

