#pragma once
#include "meta_nn/data/facilities/continuous_memory.h"
#include "meta_nn/data/facilities/lower_access.h"
#include "meta_nn/data/matrices/matrices.h"
#include "meta_nn/data/scalar.h"
#include "meta_nn/evaluates/facilities/eval_handle.h"

template <typename Elem>
struct LowerAccessImpl<Matrix<Elem, DeviceTags::CPU>>;

template <typename Elem>
class Matrix<Elem, DeviceTags::CPU>
{
public:
    static_assert(std::is_same_v<RemConstRef<Elem>, Elem>,
                  "TElem is not an available type");

    using ElementType = Elem;
    using DeviceType = DeviceTags::CPU;

    friend struct LowerAccessImpl<Matrix<Elem, DeviceTags::CPU>>;

    // TODO Batch
    // friend class Batch<TElem, DeviceTags::CPU, CategoryTags::Matrix>;

public:
    Matrix(size_t row_num = 0, size_t col_num = 0)
        : mem(row_num * col_num)
        , row_num(row_num)
        , col_num(col_num)
        , row_len(col_num)
    {
    }

    size_t RowNum() const { return row_num; }
    size_t colNum() const { return col_num; }

    void SetValue(size_t row_id, size_t col_id, ElementType val)
    {
        assert(AvailableForWrite());
        assert(row_id < row_num && col_id < col_num);
        (mem.RawMemory())[row_id * row_len + col_id] = val;
    }

    const auto operator()(size_t row_id, size_t col_id) const
    {
        assert(row_id < row_num && col_id < col_num);
        return (mem.RawMemory())[row_id * row_len + col_id];
    }

    bool AvailableForWrite() const { return mem.UseCount() == 1; }

    Matrix SubMatrix(size_t row_begin, size_t row_end, size_t col_begin, size_t col_end) const
    {
        assert(row_begin < row_num && row_end <= row_num &&
               col_begin < col_num && col_end <= col_num);

        auto pos = mem.RawMemory() + row_begin * row_len + col_begin;
        return Matrix(mem.SharedPtr(),
                      pos,
                      row_end - row_begin,
                      col_end - col_begin,
                      row_len);
    }

    // TODO
    // auto EvalRegister() const { return MakeConstEvalHandle(*this); }

private:
    Matrix(std::shared_ptr<ElementType> mem_,
           ElementType* mem_start,
           size_t row_num,
           size_t col_num,
           size_t row_len)
        : mem(mem_, mem_start)
        , row_num(row_num)
        , col_num(col_num)
        , row_len(row_len)
    {
    }

private:
    ContinuousMemory<ElementType, DeviceType> mem;
    size_t row_num;
    size_t col_num;
    size_t row_len;
};

template <typename Elem>
struct LowerAccessImpl<Matrix<Elem, DeviceTags::CPU>>
{
    LowerAccessImpl(Matrix<Elem, DeviceTags::CPU> p)
        : matrix(std::move(p))
    {
    }

    auto MutableRawMemory() { return matrix.mem.RawMemory(); }
    const auto RawMemory() const { return matrix.mem.RawMemory(); }

    size_t RowLen() const { return matrix.row_len; }

private:
    Matrix<Elem, DeviceTags::CPU> matrix;
};