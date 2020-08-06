#pragma once
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/matrices/matrices.h"
// TODO: eval

template <typename Elem, typename Device>
class ZeroMatrix
{
public:
    using ElementType = Elem;
    using DeviceType = Device;

public:
    ZeroMatrix(size_t row_num, size_t col_num)
        : row_num(row_num)
        , col_num(col_num)
    {
    }

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }

    bool operator==(const TrivalMatrix& other) const
    {
        return row_num == other.row_num &&
               col_num == other.col_num;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& val) const { return !(operator==(val)); }

    // TODO
    // auto EvalRegister() const;

private:
    size_t row_num;
    size_t col_num;
};

template <typename Elem, typename Device>
constexpr bool IsMatrix<ZeroMatrix<Elem, Device>> = true;