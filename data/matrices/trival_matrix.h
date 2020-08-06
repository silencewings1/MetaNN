#pragma once
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/matrices/matrices.h"
// TODO: eval

template <typename Elem, typename Device, typename ScalarType>
class TrivalMatrix
{
public:
    using ElementType = Elem;
    using DeviceType = Device;

public:
    TrivalMatrix(size_t row_num, size_t col_num, ScalarType val)
        : val(val)
        , row_num(row_num)
        , col_num(col_num)
    {
    }

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }
    auto ElementValue() const { return val; }

    bool operator==(const TrivalMatrix& other) const
    {
        return val == other.val &&
               row_num == other.row_num &&
               col_num == other.col_num;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& val) const { return !(operator==(val)); }

    // TODO
    // auto EvalRegister() const;

private:
    ScalarType val;
    size_t row_num;
    size_t col_num;
};

template <typename Elem, typename Device, typename ScalarType>
constexpr bool IsMatrix<TrivalMatrix<Elem, Device, ScalarType>> = true;

template <typename Elem, typename Device, typename Val>
auto MakeTrivalMatrix(size_t row_num, size_t col_num, Val&& val)
{
    using RawVal = RemConstRef<Val>;

    if constexpr (IsScalar<RawVal>)
    {
        static_assert(std::is_same_v<typename RawVal::DeviceType, Device> ||
                      std::is_same_v<typename RawVal::DeviceType, DeviceTags::CPU>);

        return TrivalMatrix<Elem, Device, RawVal>(row_num, col_num, std::forward<Val>(val));
    }
    else
    {
        auto tmp = static_cast<Elem>(val);
        Scalar<Elem, DeviceTags::CPU> scalar(std::move(tmp));

        return TrivalMatrix<Elem, Device, Scalar<Elem, DeviceTags::CPU>>(row_num, col_num, std::move(scalar));
    }
}
