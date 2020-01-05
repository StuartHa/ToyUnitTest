# Toy C++ Unit Test Framework

A toy implementation of a C++ unit test framework that is heavily inspired by [Catch2](https://github.com/catchorg/Catch2).
I created this to learn how unit tests are implemented in C++.

# Usage

```c++
#include "unit_test_toy.h"

#include <vector>

TEST_CASE("vector capacity grows with size") {
    std::vector<int> vec(5);

    REQUIRE(vec.size() == 5);
    REQUIRE(vec.capacity() >= 5);

    vec.resize(10);
    REQUIRE(vec.size() == 10);
    REQUIRE(vec.capacity() >= 10);
}
```

# Installation

```
git clone https://github.com/StuartHa/ToyUnitTest
cd ToyUnitTest
conan install .
cmake .
cmake --build .
./bin/unit_test_toy
```

