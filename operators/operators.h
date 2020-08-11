#pragma once
// #include <MetaNN/evaluate/facilities/eval_buffer.h>
#include "meta_nn/data/facilities/traits.h"
#include "meta_nn/facilities/traits.h"
#include "meta_nn/operators/facilities/category_cal.h"
#include "meta_nn/operators/facilities/oper_seq.h"
#include "meta_nn/operators/facilities/organizer.h"
#include "meta_nn/operators/facilities/traits.h"
#include "meta_nn/operators/facilities/tags.h"

/////////////////////////// UnaryOp //////////////////////////
template <typename OpTag, typename Data>
class UnaryOp : public OperOrganizer<OpTag, OperCateCal<OpTag, Data>>
{
    static_assert(std::is_same_v<RemConstRef<Data>, Data>,
                  "Data is not an available type");
    using Cate = OperCateCal<OpTag, Data>;

public:
    using ElementType = OperElementType<OpTag, Data>;
    using DeviceType = OperDeviceType<OpTag, Data>;

public:
    UnaryOp(Data data_)
        : OperOrganizer<OpTag, Cate>(data_)
        , data(std::move(data_))
    {
    }

    const Data& Operand() const { return data; }

    bool operator==(const UnaryOp& other) const { return data == other.val; }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    // TODO: Eval

private:
    Data data;

    // TODO
    // using Principal = PrincipalDataType<Cate, ElementType, DeviceType>;
    //  EvalBuffer<Principal> evalBuf;
};

template <typename OpTag, typename Data>
constexpr bool IsScalar<UnaryOp<OpTag, Data>> =
    std::is_same_v<OperCateCal<OpTag, Data>, CategoryTags::Scalar>;

template <typename OpTag, typename Data>
constexpr bool IsMatrix<UnaryOp<OpTag, Data>> =
    std::is_same_v<OperCateCal<OpTag, Data>, CategoryTags::Matrix>;

template <typename OpTag, typename Data>
constexpr bool IsBatchScalar<UnaryOp<OpTag, Data>> =
    std::is_same_v<OperCateCal<OpTag, Data>, CategoryTags::BatchScalar>;

template <typename OpTag, typename Data>
constexpr bool IsBatchMatrix<UnaryOp<OpTag, Data>> =
    std::is_same_v<OperCateCal<OpTag, Data>, CategoryTags::BatchMatrix>;

/////////////////////////// BinaryOp //////////////////////////
template <typename OpTag, typename Data1, typename Data2>
class BinaryOp : public OperOrganizer<OpTag, OperCateCal<OpTag, Data1, Data2>>
{
    static_assert(std::is_same_v<RemConstRef<Data1>, Data1>,
                  "Data1 is not an available type");
    static_assert(std::is_same_v<RemConstRef<Data2>, Data2>,
                  "Data2 is not an available type");
    using Cate = OperCateCal<OpTag, Data1, Data2>;

public:
    BinaryOp(Data1 data1_, Data2 data2_)
        : OperOrganizer<OpTag, Cate>(data1, data2)
        , data1(std::move(data1_))
        , data2(std::move(data2_))
    {
    }

    const Data1& Operand1() const { return data1; }
    const Data2& Operand2() const { return data2; }

    bool operator==(const BinaryOp& other) const
    {
        return data1 == other.data1 && data2 == other.data2;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    // TODO: Eval

private:
    Data1 data1;
    Data2 data2;

    // TODO
};

template <typename OpTag, typename Data1, typename Data2>
constexpr bool IsScalar<BinaryOp<OpTag, Data1, Data2>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2>, CategoryTags::Scalar>;

template <typename OpTag, typename Data1, typename Data2>
constexpr bool IsMatrix<BinaryOp<OpTag, Data1, Data2>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2>, CategoryTags::Matrix>;

template <typename OpTag, typename Data1, typename Data2>
constexpr bool IsBatchScalar<BinaryOp<OpTag, Data1, Data2>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2>, CategoryTags::BatchScalar>;

template <typename OpTag, typename Data1, typename Data2>
constexpr bool IsBatchMatrix<BinaryOp<OpTag, Data1, Data2>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2>, CategoryTags::BatchMatrix>;

/////////////////////////// TernaryOp //////////////////////////
template <typename OpTag, typename Data1, typename Data2, typename Data3>
class TernaryOp : public OperOrganizer<OpTag, OperCateCal<OpTag, Data1, Data2, Data3>>
{
    static_assert(std::is_same_v<RemConstRef<Data1>, Data1>,
                  "Data1 is not an available type");
    static_assert(std::is_same_v<RemConstRef<Data2>, Data2>,
                  "Data2 is not an available type");
    static_assert(std::is_same_v<RemConstRef<Data3>, Data3>,
                  "Data3 is not an available type");
    using Cate = OperCateCal<OpTag, Data1, Data2, Data3>;

public:
    TernaryOp(Data1 data1_, Data2 data2_, Data3 data3_)
        : OperOrganizer<OpTag, Cate>(data1_, data2_, date3_)
        , data1(std::move(data1_))
        , data2(std::move(data2_))
        , data3(std::move(data3_))
    {
    }

    const Data1& Operand1() const { return data1; }
    const Data2& Operand2() const { return data2; }
    const Data3& Operand3() const { return data3; }

    bool operator==(const BinaryOp& other) const
    {
        return data1 == other.data1 &&
               data2 == other.data2 &&
               data3 == other.data3;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    // TODO: Eval

private:
    Data1 data1;
    Data2 data2;
    Data3 Date3;

    // TODO
};

template <typename OpTag, typename Data1, typename Data2, typename Data3>
constexpr bool IsScalar<TernaryOp<OpTag, Data1, Data2, Data3>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2, Data3>, CategoryTags::Scalar>;

template <typename OpTag, typename Data1, typename Data2, typename Data3>
constexpr bool IsMatrix<TernaryOp<OpTag, Data1, Data2, Data3>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2, Data3>, CategoryTags::Matrix>;

template <typename OpTag, typename Data1, typename Data2, typename Data3>
constexpr bool IsBatchScalar<TernaryOp<OpTag, Data1, Data2, Data3>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2, Data3>, CategoryTags::BatchScalar>;

template <typename OpTag, typename Data1, typename Data2, typename Data3>
constexpr bool IsBatchMatrix<TernaryOp<OpTag, Data1, Data2, Data3>> =
    std::is_same_v<OperCateCal<OpTag, Data1, Data2, Data3>, CategoryTags::BatchMatrix>;