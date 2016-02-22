#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch/catch.hpp"

#include <core/string_view.hpp>

#include "../common/utils.h"
#include "../common/api.h"
#include "../common/client_api.h"
#include "../common/file_scanner.h"
#include "../common/file_diff.h"

TEST_CASE("Bytification of ints", "[serialization]") {
    int i = 0;
    auto bytes = to_bytes(i);
    REQUIRE(bytes == (std::vector<char>{0, 0, 0, 0}));

    i = 0xAABBCCDD;
    bytes = to_bytes(i);
    REQUIRE(bytes == (std::vector<char>{char(0xDD), char(0xCC), char(0xBB), char(0xAA)}));
}

TEST_CASE("Debytification of ints", "[serialization]") {
    std::vector<char> bytes{0, 0, 0, 0};
    int i;
    from_bytes(bytes.begin(), bytes.end(), i);
    REQUIRE(i == 0);

    bytes = std::vector<char>{char(0xDD), char(0xCC), char(0xBB), char(0xAA)};
    from_bytes(bytes.begin(), bytes.end(), i);
    REQUIRE(i == 0xAABBCCDD);
}

TEST_CASE("Time roundtrip", "[serialization]") {
    const auto now = time(nullptr);
    auto bytes = ::to_bytes(now);
    time_t back;
    from_bytes(bytes.begin(), bytes.end(), back);
    REQUIRE(now == back);
}

TEST_CASE("Debytification of string", "[serialization]") {
    std::vector<char> bytes{'a', 'b', 'c', 0, char(0xDD), char(0xCC), char(0xBB), char(0xAA)};
    std::string s;
    int i;
    auto it = from_bytes(bytes.begin(), bytes.end(), s);
    (void) from_bytes(it, bytes.end(), i);
    REQUIRE(s == "abc");
    REQUIRE(i == 0xAABBCCDD);
}

TEST_CASE("Debytification of strings", "[serialization]") {
    std::vector<char> bytes{'a', 'b', 'c', 0, char(0xDD), char(0xCC), char(0xBB), char(0xAA),
                            'd', 'e', 'f', 0};
    std::string s1, s2;
    int i;
    auto it = from_bytes(bytes.begin(), bytes.end(), s1);
    it = from_bytes(it, bytes.end(), i);
    (void) from_bytes(it, bytes.end(), s2);
    REQUIRE(s1 == "abc");
    REQUIRE(i == 0xAABBCCDD);
    REQUIRE(s2 == "def");
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
    std::vector<char> bytes = {0, 1, 2, 0, 0, 0, 3, 0, 0, 0, 'a', 'p', 'i'};
    Api api(bytes);
    REQUIRE(api.getType() == 0);
    REQUIRE(api.getStatus() == 1);
    REQUIRE(api.getID() == 2);
    REQUIRE(api.getPayload() == (std::vector<char>{'a', 'p', 'i'}));
}

TEST_CASE("Api bytification", "[api]") {
    Api api(0, 1, 2, {'a', 'p', 'i'});
    std::vector<char> bytes = {0, 1, 2, 0, 0, 0, 3, 0, 0, 0, 'a', 'p', 'i'};
    REQUIRE(api.to_bytes() == bytes);
}

TEST_CASE("GetTime construction", "[api]") {
    GetTime gt(123);
    std::vector<char> bytes = {32, 0, 123, 0, 0, 0, /*payload size*/ 0, 0, 0, 0};
    REQUIRE(gt.to_bytes() == bytes);
}

TEST_CASE("GetTime from bytes", "[api]") {
    std::vector<char> bytes = {32, 0, 21, 0, 0, 0, /*payload size*/ 0, 0, 0, 0};
    GetTime gt(bytes);
    REQUIRE(gt.getType() == 32);
    REQUIRE(gt.getStatus() == 0);
    REQUIRE(gt.getID() == 21);
    REQUIRE(gt.getName() == "GetTime");
}

TEST_CASE("GetFileList construction", "[api]") {
    GetFileList gfl(123);
    std::vector<char> bytes = {34, 0, 123, 0, 0, 0, /*payload size*/ 0, 0, 0, 0};
    REQUIRE(gfl.to_bytes() == bytes);
}

TEST_CASE("GetFileList from bytes", "[api]") {
    std::vector<char> bytes = {34, 0, 21, 0, 0, 0, /*payload size*/ 0, 0, 0, 0};
    GetFileList gfl(bytes);
    REQUIRE(gfl.getType() == 34);
    REQUIRE(gfl.getStatus() == 0);
    REQUIRE(gfl.getID() == 21);
    REQUIRE(gfl.getName() == "GetFileList");
}

TEST_CASE("GetFileByPath construction", "[api]") {
    GetFileByPath gfbp(123, "abc.txt");
    std::vector<char> bytes = {36, 0, 123, 0, 0, 0, /*payload size*/ 8, 0, 0, 0,
    'a', 'b', 'c', '.', 't', 'x', 't', '\0'};
    REQUIRE(gfbp.to_bytes() == bytes);
}

TEST_CASE("GetFileByPath from bytes", "[api]") {
    std::vector<char> bytes = {36, 0, 21, 0, 0, 0, /*payload size*/ 7, 0, 0, 0,
    'x', 'D', '.', 'w', 't', 'f', '\0'};
    GetFileByPath gfbp(bytes);
    REQUIRE(gfbp.getType() == 36);
    REQUIRE(gfbp.getStatus() == 0);
    REQUIRE(gfbp.getID() == 21);
    REQUIRE(gfbp.getName() == "GetFileByPath");
    auto s = gfbp.getPath().to_string();
    REQUIRE(s.size() == 6);
    REQUIRE(gfbp.getPath() == "xD.wtf");
}

TEST_CASE("SendFileToServer from file", "[api]") {
    auto now = time(nullptr);
    SendFileToServer send(13, "aaa.txt", "./test_dir_original", now);
    REQUIRE(send.getPath() == "aaa.txt");
    REQUIRE(send.getTimestamp() == now);

    // xxd -p test_dir_original/aaa.txt
    REQUIRE(send.getFile() == hex_to_bytes("616c61206d61206b6f7461"));

    auto bytes = send.to_bytes();
    SendFileToServer second(bytes);
    send.dumpPayload();
    second.dumpPayload();
    REQUIRE(second.getPath() == "aaa.txt");
    REQUIRE(second.getTimestamp() == now);
    REQUIRE(second.getFile() == hex_to_bytes("616c61206d61206b6f7461"));
}

TEST_CASE("MarkAsDeleted roundtrip", "[api]") {
    MarkAsDeleted deleted(123, "abc.txt");
    std::vector<char> bytes = {42, 0, 123, 0, 0, 0, /*payload size*/ 8, 0, 0, 0,
                               'a', 'b', 'c', '.', 't', 'x', 't', '\0'};
    REQUIRE(deleted.getPath() == "abc.txt");
    REQUIRE(deleted.to_bytes() == bytes);

    MarkAsDeleted second(bytes);
    REQUIRE(second.getPath() == "abc.txt");
    REQUIRE(deleted.to_bytes() == second.to_bytes());
}

TEST_CASE("FileScanner full directory scan", "[file_scanner]") {
    FileScanner sc("./test_dir_original");

    auto files = std::set<std::string>();

    files.insert(sc.getFileInfo()[0].path);
    files.insert(sc.getFileInfo()[1].path);
    files.insert(sc.getFileInfo()[2].path);

    REQUIRE(files.find("aaa.txt") != files.end());
    REQUIRE(files.find("bbb/ddd/eee.txt") != files.end());
    REQUIRE(files.find("bbb/ccc.txt") != files.end());
}

TEST_CASE("Files as list", "[file_scanner]") {
    FileScanner sc("./test_dir_original");
    std::string list = sc.asFileList();
    REQUIRE(list.find("aaa.txt") != std::string::npos);
    REQUIRE(list.find("bbb/ddd/eee.txt") != std::string::npos);
    REQUIRE(list.find("bbb/ccc.txt") != std::string::npos);
}

TEST_CASE("FileScanner roundtrip", "[file_scanner]") {
    FileScanner fs("./test_dir_original");
    std::string list = fs.asFileList();
    auto bytes = to_bytes(list);
    FileScanner second(bytes);
    REQUIRE(fs.getFileInfo() == second.getFileInfo());
}

TEST_CASE("FileDiff additions", "[file_diff]") {
    std::vector<FileInfo> mine = {{"aaa.txt", 1}, {"bbb.txt", 2}};
    std::vector<FileInfo> remote = {{"aaa.txt", 1}, {"ccc.txt", 3}};
    FileDiff fd(mine, remote);
    auto added = fd.getAdded();
    REQUIRE(added.size() == 1);
    REQUIRE((added.front() == FileInfo{"ccc.txt", 3}));
}

TEST_CASE("FileDiff deletions", "[file_diff]") {
    std::vector<FileInfo> mine = {{"aaa.txt", 1}, {"bbb.txt", 2}};
    std::vector<FileInfo> remote = {{"aaa.txt", 1}, {"ccc.txt", 3}};
    FileDiff fd(mine, remote);
    auto deleted = fd.getDeleted();
    REQUIRE(deleted.size() == 1);
    REQUIRE((deleted.front() == FileInfo{"bbb.txt", 2}));
}

TEST_CASE("FileDiff modifications", "[file_diff]") {
    std::vector<FileInfo> mine = {{"aaa.txt", 1}, {"bbb.txt", 2}};
    std::vector<FileInfo> remote = {{"aaa.txt", 4}, {"bbb.txt", 2}};
    FileDiff fd(mine, remote);
    auto modified = fd.getModifiedOrAdded();
    REQUIRE(modified.size() == 1);
    REQUIRE((modified.front() == FileInfo{"aaa.txt", 4})); // Returns newer timestamp
}
