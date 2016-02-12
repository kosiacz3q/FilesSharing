#pragma once

#include <vector>
#include <cstring>
#include <cassert>
#include <type_traits>
#include <mutex>

template<typename T>
std::vector<char> to_bytes(const T& payload) {
    std::vector<char> buffer(sizeof(payload));
    memcpy(buffer.data(), (const void*) &payload, sizeof(payload));
}

template<typename T>
T from_bytes(const std::vector<char>& buffer) {
    assert(buffer.size() == sizeof(T));
    T t;
    memcpy(&t, buffer.data(), buffer.size());
    return t;
}

template<bool V, typename True, typename False>
using if_t = std::conditional_t<V, True, False>;

using mutex_guard = std::lock_guard<std::mutex>;