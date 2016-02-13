#pragma once

#include <vector>
#include <cstring>
#include <cassert>
#include <type_traits>
#include <mutex>
#include <utility>
#include <tuple>

inline std::vector<char> join_vectors(const std::vector<std::reference_wrapper<
        const std::vector<char>>>& list) {
    std::vector<char> result;
    for (auto& x : list)
        result.insert(result.end(), x.get().begin(), x.get().end());
    return result;
}

template<typename T>
std::vector<char> to_bytes_impl(const T& payload) {
    static_assert(std::is_trivially_copyable<T>(), "");
    std::vector<char> buffer(sizeof(payload));
    memcpy(buffer.data(), (const void*) &payload, sizeof(payload));
    return buffer;
}

inline std::vector<char> to_bytes() {
    return {};
}

template<typename First, typename... Rest>
std::vector<char> to_bytes(const First& first, const Rest&... rest) {
    auto firstBytes = to_bytes_impl(first);
    auto restBytes = to_bytes(rest...);
    return join_vectors({firstBytes, restBytes});
}

template<typename T, typename IT>
void from_bytes_impl(T& t, IT begin) {
    static_assert(std::is_trivially_copyable<T>(), "");
    static_assert(std::is_default_constructible<T>(), "");
    memcpy(&t, &*begin, sizeof(T));
};

template<typename...>
struct total_size;

template<>
struct total_size<> : std::integral_constant<size_t, 0> {};

template<typename First, typename...Rest>
struct total_size<First, Rest...> : std::integral_constant<size_t,
                                        sizeof(First) + total_size<Rest...>::value> {};

template<typename IT>
IT from_bytes(IT begin, IT end) {
    (void) end;
    return begin;
}

template<typename First, typename... Rest, typename IT>
IT from_bytes(IT begin, IT end, First& first, Rest&... rest) {
    constexpr size_t total_bytes = total_size<First, Rest...>::value;
    assert((char*) &*end - (char*) &*begin >= total_bytes);
    assert(begin < end);
    (void) end;
    (void) total_bytes;
    from_bytes_impl(first, begin);
    return from_bytes(begin + sizeof(First)/sizeof(*begin), end, rest...);
};

template<bool V, typename True, typename False>
using if_t = std::conditional_t<V, True, False>;

using mutex_guard = std::lock_guard<std::mutex>;
