#pragma once
#include "meta_nn/operators/operators.h"

template <typename T>
struct OperSigmoid_
{
private:
    using RawType = RemConstRef<T>;

public:
    constexpr static bool vaild = IsMatrix<RawType> || IsBatchMatrix<RawType>;

public:
    static auto Eval(T&& para)
    {
        using ResType = UnaryOp<UnaryOpTags::Sigmoid, RawType>;
        return ResType(std::forward<T>(para));
    }
};

template <typename T, std::enable_if_t<OperSigmoid_<T>::vaild>* = nullptr>
auto Sigmoid(T&& para)
{
    return OperSigmoid_<T>::Eval(std::forward<T>(para));
}