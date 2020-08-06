#pragma once
#include "meta_nn/data/facilities/continuous_memory.h"
#include "meta_nn/data/facilities/lower_access.h"
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/facilities/traits.h"

template <typename Elem, typename Device, typename Category>
class Batch;

template <typename Elem, typename Device>
constexpr bool IsBatchMatrix<Batch<Elem, Device, CategoryTags::Matrix>> = true;

template <typename Elem, typename Device>
constexpr bool IsBatchScalar<Batch<Elem, Device, CategoryTags::Scalar>> = true;
