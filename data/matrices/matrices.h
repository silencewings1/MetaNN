#pragma once
#include "meta_nn/data/facilities/traits.h"

template <typename Elem, typename Device>
class Matrix;

template <typename Elem, typename Device>
constexpr bool IsMatrix<Matrix<Elem, Device>> = true;