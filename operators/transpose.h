#pragma once
#include "meta_nn/operators/operators.h"

template <>
class OperOrganizer<UnaryOpTags::Transpose, CategoryTags::Matrix>
{
public:
    template <typename Data>
    OperOrganizer(const Data& data)
        : row_num(data.RowNum())
        , col_num(data.ColNum())
    {
    }

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }

private:
    size_t row_num;
    size_t col_num;
};

template <>
class OperOrganizer<UnaryOpTags::Transpose, CategoryTags::BatchMatrix>
    : public OperOrganizer<UnaryOpTags::Transpose, CategoryTags::Matrix>
{
    using Base = OperOrganizer<UnaryOpTags::Transpose, CategoryTags::Matrix>;

public:
    template <typename Data>
    OperOrganizer(const Data& data)
        : Base(data)
        , batch_num(data.BatchNum())
    {
    }

private:
    size_t batch_num;
};

template <typename T>
struct OperTranspose_
{
private:
    using RawT = RemConstRef<T>;

public:
    static constexpr bool vaild = IsMatrix<RawT> || IsBatchMatrix<RawT>;

public:
    static auto Eval(T&& data)
    {
        using ResType = UnaryOp<UnaryOpTags::Transpose, RawT>;
        return ResType(std::forward<T>(data));
    }
};

template <typename T,
          std::enable_if_t<OperTranspose_<T>::vaild>* = nullptr>
auto Transpose(T&& data)
{
    return OperTranspose_<T>::Eval(std::forward<T>(data);)
}