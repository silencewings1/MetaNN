#pragma once
#include <cstddef>


template <typename T>
using RemConstRef = std::remove_cv_t<std::remove_reference_t<T>>;