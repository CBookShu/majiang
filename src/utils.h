#pragma once
#include <iostream>
#include <cstring>
#include <type_traits>
#include <cassert>

template <typename T>
static void zero_struct(T&& t) {
    using PureT = std::decay_t<decltype(std::forward<T>(t))>;
    if constexpr (std::is_pointer_v<PureT>) {
        static_assert(std::is_pod_v<std::remove_pointer_t<PureT>>);
        std::memset(t, 0, sizeof(*t));
    } else {
        static_assert(std::is_pod_v<PureT>);
        std::memset(&t, 0, sizeof(t));
    }
}

template <typename T, int N>
struct c_static_array {
    T t[N];
    int count;

    auto grap() ->T* {
        assert(count < N);
        return &t[count++];
    }
    auto pop() ->void {
        count--;
    }
    auto cur() ->T* {
        if(count == 0) {
            return nullptr;
        }
        return &t[count-1];
    }
    void push(T&& t) {
        *grap() = std::forward<T>(t);
    }
    void push(T& t) {
        *grap() = t;
    }
    auto pat(int pos)->T* {
        return &t[pos];
    }
    // support op like c array
    T& operator[](int pos) {
        return t[pos];
    }

    // to support std foreach
    auto begin() ->T* {
        return &t[0];
    }
    auto end() ->T* {
        return &t[count];
    }
};