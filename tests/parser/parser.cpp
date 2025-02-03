#include "paracl/ast/ast.h"
#include "catch2/catch2.h"


TEST_CASE("parse tokens") {
    using namespace paracl;

    SECTION("no tokens") {
        std::vector<token> tokens = {};

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE("main( )" == oss.str());
    }
}