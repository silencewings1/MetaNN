#pragma once
#include <cstddef>


template <typename T>
using RemConstRef = std::remove_cv_t<std::remove_reference_t<T>>;

// And
template <bool cur, typename Next>
constexpr static bool AndValue = false;

template <typename Next>
constexpr static bool AndValue<true, Next> = Next::value;

// Or
template <bool cur, typename Next>
constexpr static bool OrValue = true;

template <typename Next>
constexpr static bool OrValue<false, Next> = Next::value;