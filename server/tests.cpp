
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch/catch.hpp"

#include <core/string_view.hpp>

#include <common/server_api.h>

TEST_CASE("ServerDeletedList single bytefication"){

    auto sdl = ServerDeletedList(100, {"somePath"});

    auto bytes = std::vector<char>{
            41, 0,
            100, 0, 0, 0,
            9, 0, 0, 0,
            's', 'o', 'm', 'e', 'P', 'a', 't', 'h', '\0'
    };

    REQUIRE(sdl.to_bytes() == bytes);;
}

TEST_CASE("ServerDeletedList several bytefication"){

    auto sdl = ServerDeletedList(100, {"somePath1", "somePath2"});

    auto bytes = std::vector<char>{
            41, 0,
            100, 0, 0, 0,
            20, 0, 0, 0,
            's', 'o', 'm', 'e', 'P', 'a', 't', 'h', '1', '\0',
            's', 'o', 'm', 'e', 'P', 'a', 't', 'h', '2', '\0'
    };

    REQUIRE(sdl.to_bytes() == bytes);;
}

TEST_CASE("ServerDeletedList from bytes"){

    auto sdl = ServerDeletedList(100, {"somePath1", "somePath2"});


    auto sdl2 = ServerDeletedList(sdl.to_bytes());

    auto result = std::vector<std::string>{"somePath1", "somePath2"};

    REQUIRE(sdl2.getDeletedList().size() == 2);
    REQUIRE(sdl2.getDeletedList() == result);
}


TEST_CASE("FileFromServer empty"){

    auto f = FileFromServer(1,1);

    auto bytes = std::vector<char> { 37, 1,
                                     1, 0, 0, 0,
                                     0, 0, 0, 0};

    REQUIRE(f.to_bytes() == bytes);
}