#include "unit_test_toy.h"

#include <vector>

using std::vector;

TEST_CASE("vector capacity grows with size") {
    vector<int> vec(5);

    REQUIRE(vec.size() == 5);
    REQUIRE(vec.capacity() >= 5);

    vec.resize(10);
    REQUIRE(vec.size() == 10);
    REQUIRE(vec.capacity() >= 10);
}

TEST_CASE("vector size becomes zero on clear") {
    vector<int> vec(5);
    REQUIRE(vec.size() > 0);

    vec.clear();

    // Example failure.
    REQUIRE(vec.size() == 5);
}
