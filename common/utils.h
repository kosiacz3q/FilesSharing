#pragma once

#include <vector>
#include <cstring>
#include <cassert>
#include <type_traits>
#include <mutex>
#include <utility>
#include <tuple>
#include <algorithm>
#include <memory>
#include <initializer_list>
#include <core/string_view.hpp>

inline std::vector<char> join_vectors(const std::initializer_list<std::vector<char>> list) {
    std::vector<char> result;
    for (auto& x : list)
        result.insert(result.end(), x.begin(), x.end());
    return result;
}

template<typename T>
std::vector<char> to_bytes_impl(const T& payload) {
    static_assert(std::is_trivially_copyable<T>(), "");
    std::vector<char> buffer(sizeof(payload));
    memcpy(buffer.data(), (const void*) &payload, sizeof(payload));
    return buffer;
}

inline std::vector<char> to_bytes_impl(core::string_view text) {
    std::vector<char> buffer(text.size() + 1);
    std::copy(text.begin(), text.end(), buffer.begin());
    buffer.back() = '\0';
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

template<typename T>
struct type_wrapper {
    using type = T;
};

template <typename Tuple, typename F, std::size_t... Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void) swallow{1, (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), 0)...};
}

template <typename Tuple, typename F>
void for_each_tuple(Tuple&& tuple, F&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                  std::make_index_sequence<N>{});
}

template<typename, template<typename> class>
struct static_for_each_impl;

template<typename... Ts, template<typename> class F>
struct static_for_each_impl<std::tuple<Ts...>, F> {
    using type = std::tuple<F<Ts>...>;
};

template<typename T, template<typename> class F>
using static_for_each = typename static_for_each_impl<T, F>::type;

template <typename T_DEST, typename T_SRC, typename Deleter>
std::unique_ptr<T_DEST, Deleter> unique_cast(std::unique_ptr<T_SRC, Deleter> src) {
    if (!src) return nullptr;

    T_DEST* dest_ptr = dynamic_cast<T_DEST*>(src.get());
    src.release();
    return std::unique_ptr<T_DEST, Deleter>(dest_ptr);
}

template<typename T>
struct DebugTypeTeller; // no impl - substitution always fails

template<typename T>
void debugTellType(T) {
    DebugTypeTeller<T> t;
    (void) t;
}

template<typename T>
void debugTellType() {
    DebugTypeTeller<T> t;
    (void) t;
}
