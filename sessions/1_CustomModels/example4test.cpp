#include <limits>
#include "example4src.cpp"
#include "catch_amalgamated.hpp"

TEST_CASE(
    "F: Execution",
    "[F]"
){
    REQUIRE_NOTHROW ( F(1) );
    REQUIRE ( std::isinf(F(0)) );

}

TEST_CASE(
    "F: Result",
    "[F]"
){
    REQUIRE ( F(2) == 4.0 );
    Catch::Approx target = Catch::Approx(12.0).epsilon(0.1);
    REQUIRE ( F(5.86) == target );
}

TEST_CASE(
    "F: Simple Benchmarking",
    "[F]"
){
    BENCHMARK( "0.1" ) { return F(0.1); };
    BENCHMARK( "10" ) { return F(10); };
    BENCHMARK( "-7" ) { return F(-7); };
}

TEST_CASE(
    "G: Fix-RNG",
    "[G]"
){
    REQUIRE ( G(0) == 31 );
}
