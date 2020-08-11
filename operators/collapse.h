#pragma once
#include "meta_nn/operators/operators.h"

template <>
struct OperCategory_<UnaryOpTags::Collapse, CategoryTags::BatchMatrix>
{
    using type = CategoryTags::Matrix;
};

template <typename T>
struct OperCollapse_
{
private:
    using RawT = std::decay_t<T>;

public:
    static constexpr bool vaild = IsBatchMatrix<RawT>;

public:
    static auto Eval(T&& data)
    {
        using ResType = UnaryOp<UnaryOpTags::Collapse, RawT>;
        return ResType(std::forward<T>(data));
    }
};

template <typename T,
          std::enable_if_t<OperCollapse_<T>::vaild>* = nullptr>
auto Collapse(T&& data)
{
    return OperCollapse_<T>::Eval(std::forward<T>(data));
}