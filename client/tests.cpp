#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch/catch.hpp"

#include "../common/utils.h"

TEST_CASE("Bytification of ints", "[to_byte]") {
    int i = 0;
    auto bytes = to_bytes(i);
    REQUIRE(bytes == (std::vector<char>{0, 0, 0, 0}));

    i = 0xAABBCCDD;
    bytes = to_bytes(i);
    REQUIRE(bytes == (std::vector<char>{char(0xDD), char(0xCC), char(0xBB), char(0xAA)}));
}

TEST_CASE("Debytification of ints", "[from_byte]") {
    std::vector<char> bytes{0, 0, 0, 0};
    int i;
    from_bytes(i, bytes.begin());
    REQUIRE(i == 0);

    bytes = std::vector<char>{char(0xDD), char(0xCC), char(0xBB), char(0xAA)};
    from_bytes(i, bytes.begin());
    REQUIRE(i == 0xAABBCCDD);
}

TEST_CASE("Debytification of ints p.2", "[from_byte]") {
    std::vector<char> bytes{0, 0, 0, 0};
    auto i = from_bytes<int>(bytes.begin(), bytes.end());
    REQUIRE(i == 0);

    bytes = std::vector<char>{char(0xDD), char(0xCC), char(0xBB), char(0xAA)};
    i = from_bytes<int>(bytes.begin(), bytes.end());
    REQUIRE(i == 0xAABBCCDD);
}

TEST_CASE("Joining vectors", "[join_vector]") {
    std::vector<char> first{0, 0, 0};
    std::vector<char> second{1, 1};
    std::vector<char> third{2};
    REQUIRE(join_vectors({first, second, third}) == (std::vector<char>{0, 0, 0, 1, 1, 2}));
}