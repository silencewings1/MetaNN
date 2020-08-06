#pragma once
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/matrices/matrices.h"
// TODO: eval

template <typename Elem, typename Device>
class OneHotVector
{
public:
    using ElementType = Elem;
    using DeviceType = Device;

public:
    OneHotVector(size_t col_num, size_t hot_pos)
        : col_num(col_num)
        , hot_pos(hot_pos)
    {
        assert(hot_pos < col_num);
    }

    size_t RowNum() const { return 1; }
    size_t ColNum() const { return col_num; }
    size_t HotPos() const { return hot_pos; }

    bool operator==(const OneHotVector& other) const
    {
        return col_num == other.col_num &&
               hot_pos == other.hot_pos;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& val) const { return !(operator==(val);) }

private:
    size_t col_num;
    size_t hot_pos;
};

template <typename Elem, typename Device>
constexpr bool IsMatrix<OneHotVector<Elem, Device>> = true;