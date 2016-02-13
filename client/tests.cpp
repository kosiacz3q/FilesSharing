#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch/catch.hpp"

#include "../common/utils.h"
#include "../common/api.h"

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
    from_bytes(bytes.begin(), bytes.end(), i);
    REQUIRE(i == 0);

    bytes = std::vector<char>{char(0xDD), char(0xCC), char(0xBB), char(0xAA)};
    from_bytes(bytes.begin(), bytes.end(), i);
    REQUIRE(i == 0xAABBCCDD);
}

TEST_CASE("Joining vectors", "[join_vector]") {
    std::vector<char> first{0, 0, 0};
    std::vector<char> second{1, 1};
    std::vector<char> third{2};
    REQUIRE(join_vectors({first, second, third}) == (std::vector<char>{0, 0, 0, 1, 1, 2}));
}

TEST_CASE("Api construction", "[api]") {
    Api api(0, 1, 2);
    REQUIRE(api.getType() == 0);
    REQUIRE(api.getStatus() == 1);
    REQUIRE(api.getID() == 2);
    REQUIRE(api.getPayload().size() == 0);
}

TEST_CASE("Api construction with payload", "[api]") {
    Api api(0, 1, 2, {'a', 'p', 'i',});
    REQUIRE(api.getType() == 0);
    REQUIRE(api.getStatus() == 1);
    REQUIRE(api.getID() == 2);
    REQUIRE(api.getPayload() == (std::vector<char>{'a', 'p', 'i'}));
}

TEST_CASE("Api construction with bytes", "[api]") {
    std::vector<char> bytes = {0, 1, 2, 0, 0, 0, 'a', 'p', 'i'};
    Api api(bytes);
    REQUIRE(api.getType() == 0);
    REQUIRE(api.getStatus() == 1);
    REQUIRE(api.getID() == 2);
    REQUIRE(api.getPayload() == (std::vector<char>{'a', 'p', 'i'}));
}

TEST_CASE("Api bytification", "[api]") {
    Api api(0, 1, 2, {'a', 'p', 'i'});
    std::vector<char> bytes = {0, 1, 2, 0, 0, 0, 'a', 'p', 'i'};
    REQUIRE(api.to_bytes() == bytes);
}