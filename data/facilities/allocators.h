#pragma once
#include "meta_nn/data/facilities/tags.h"
#include <memory>

template <typename Device>
struct Allocator;

template <>
struct Allocator<DeviceTags::CPU>
{
    template <typename ElemType>
    static std::shared_ptr<ElemType> Allocate(size_t elem_size)
    {
        return std::shared_ptr<ElemType>(new ElemType[elem_size],
                                         [](ElemType* ptr) { delete[] ptr; });
    }
};