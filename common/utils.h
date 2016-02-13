#pragma once

#include <vector>
#include <cstring>
#include <cassert>
#include <type_traits>
#include <mutex>

template<typename T>
std::vector<char> to_bytes(const T& payload) {
    static_assert(std::is_trivially_copyable<T>(), "");
    std::vector<char> buffer(sizeof(payload));
    memcpy(buffer.data(), (const void*) &payload, sizeof(payload));
    return buffer;
}

template<typename T, typename IT>
T from_bytes(IT begin, IT end) {
    static_assert(std::is_trivially_copyable<T>(), "");
    static_assert(std::is_default_constructible<T>(), "");
    assert((char*) end - (char*) begin == sizeof(T));
    (void) end;
    T t;
    memcpy(&t, &*begin, sizeof(T));
    return t;
}

template<typename T, typename IT>
void from_bytes(T& t, IT begin) {
    static_assert(std::is_trivially_copyable<T>(), "");
    static_assert(std::is_default_constructible<T>(), "");
    memcpy(&t, &*begin, sizeof(T));
};

inline std::vector<char> join_vectors(const std::vector<const std::vector<char>*>& list) {
    std::vector<char> result;
    for (auto& x : list) {
        assert(x);
        result.insert(result.end(), x->begin(), x->end());
    }
    return result;
}

template<bool V, typename True, typename False>
using if_t = std::conditional_t<V, True, False>;

using mutex_guard = std::lock_guard<std::mutex>;
