#pragma once
#include "meta_nn/data/batch/batch.h"
#include "meta_nn/data/matrices/matrices.h"

template <typename Elem, typename Device>
struct LowerAccessImpl<Batch<Elem, Device, CategoryTags::Matrix>>;

template <typename Elem, typename Device>
class Batch<Elem, Device, CategoryTags::Matrix>
{
public:
    using ElementType = Elem;
    using DeviceType = Device;

    friend struct LowerAccessImpl<Batch<Elem, Device, CategoryTags::Matrix>>;

public:
    Batch(size_t batch_num = 0, size_t row_num = 0, size_t col_num = 0)
        : mem(row_num * col_num * batch_num)
        , row_num(row_num)
        , col_num(col_num)
        , batch_num(batch_num)
        , row_len(col_num)
        , raw_matrix_size(row_num * col_num)
    {
    }

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }
    size_t BatchNum() const { return batch_num; }

    bool AvailableForWrite() const { return mem.UseCount() == 1; }

    void SetValue(size_t batch_id, size_t row_id, size_t col_id, ElementType val)
    {
        assert(AvailableForWrite());
        assert(batch_id < batch_num &&
               row_id < row_num &&
               col_id < col_num);

        size_t pos = batch_id * raw_matrix_size + col_id + row_len * row_id;
        (mem.RawMemory())[pos] = val;
    }

    const auto operator[](size_t batch_id) const
    {
        assert(batch_id < batch_num);

        auto pos = mem.RawMemory() + batch_id * raw_matrix_size;
        return Matrix<ElementType, DeviceType>(mem.SharedPtr(), pos,
                                               row_num, col_num, row_len);
    }

    auto SubBatchMatrix(size_t row_begin, size_T row_end, size_t col_begin, size_t col_end) const
    {
        assert(row_begin < row_num && row_end <= row_num &&
               col_begin < col_num && col_end <= col_num);

        auto pos = mem.RawMemor() + col_begin + row_begin * col_num;
        return Batch(mem.SharedPtr(), pos,
                     row_end - row_begin,
                     col_end - col_begin,
                     batch_num,
                     row_len,
                     raw_matrix_size);
    }

    bool operator==(const Batch& other) const
    {
        return mem == other.mem &&
               row_num == other.row_num &&
               col_num == other.col_num &&
               row_len == other.row_len &&
               raw_matrix_size == other.raw_matrix_size;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    // Eval

private:
    Batch(std::shared_ptr<ElementType> mem_,
          ElementType* mem_start,
          size_t row_num,
          size_t col_num,
          size_t batch_num,
          size_t row_len,
          size_t matrix_size)
        : mem(mem_, mem_start)
        , row_num(row_num)
        , col_num(col_num)
        , batch_num(batch_num)
        , row_len(row_len)
        , raw_matrix_size(matrix_size)
    {
    }

private:
    ContinuousMemory<Elem, Device> mem;
    size_t row_num;
    size_t col_num;
    size_t batch_num;
    size_t row_len;
    size_t raw_matrix_size;
};

template <typename Elem, typename Device>
struct LowerAccessImpl<Batch<Elem, Device, CategoryTags::Matrix>>
{
private:
    using BatchType = Batch<Elem, Device, CategoryTags::Matrix>;

public:
    LowerAccessImpl(BatchType p)
        : data(std::move(p))
    {
    }

    auto MutableRawMemory() { return data.mem.RawMemory(); }
    const auto RawMemory() const { return data.mem.RawMemory(); }

    size_t RowLen() const { return data.row_len; }
    size_t RawMatrixSize() const { return data.raw_matrix_size; }

private:
    BatchType data;
};