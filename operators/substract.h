#pragma once
#include "meta_nn/data/batch/duplicate.h"
#include "meta_nn/data/matrices/trival_matrix.h"
#include "meta_nn/data/scalar.h"
#include "meta_nn/operators/operators.h"

template <typename T1, typename T2>
struct OperSubstract_
{
private:
    using RawT1 = RemConstRef<T1>;
    using RawT2 = RemConstRef<T2>;

public:
    static constexpr bool vaild = (IsMatrix<RawT1> && IsMatrix<RawT2>) ||
                                  (IsMatrix<RawT1> && IsScalar<RawT2>) ||
                                  (IsScalar<RawT1> && IsMatrix<RawT2>) ||
                                  (IsScalar<RawT1> && IsBatchMatrix<RawT2>) ||
                                  (IsBatchMatrix<RawT1> && IsScalar<RawT2>) ||
                                  (IsMatrix<RawT1> && IsBatchMatrix<RawT2>) ||
                                  (IsBatchMatrix<RawT1> && IsMatrix<RawT2>) ||
                                  (IsBatchMatrix<RawT1> && IsBatchMatrix<RawT2>);

public:
    template <typename C1, typename C2,
              std::enable_if_t<std::is_same_v<C1, C2>> *= nullptr>
    static auto Eval(T1&& data1, T2&& data2)
    {
        static_assert(std::is_same_v<typename RawT1::ElementType, typename RawT2::ElementType>,
                      "Matrices with different element types cannot substract directly");
        static_assert(std::is_same_v<typename RawT1::DeviceType, typename RawT2::DeviceType>,
                      "Matrices with different device types cannot substract directly");

        using ResType = BinaryOp<BinaryOpTags::Substract, RawT1, RawT2>;
        return ResType(std::forward<T1>(data1), std::forward<T2>(data2));
    }

    template <typename C1, typename C2,
              std::enable_if_t<std::is_same_v<C1, CategoryTags::Matrix>> *= nullptr,
              std::enable_if_t<std::is_same_v<C2, CategoryTags::Scalar>> *= nullptr>
    static auto Eval(T1&& data1, T2&& data2)
    {
        using ElementType = RawT1::ElementType;
        using DeviceType = RawT2::DeviceType;

        auto tmp_mat = MakeTrivalMatrix<ElementType, DeviceType>(
            data1.RowNum(), data1.ColNum(), std::forward<T2>(data2));

        using ResType = BinaryOp<BinaryOpTags::Substract,
                                 RawT1,
                                 RemConstRef<decltype(tmp_mat)>>;
        return ResType(std::forward<T1>(data1), std::move(tmp_mat));
    }

    template <typename C1, typename C2,
              std::enable_if_t<std::is_same_v<C1, CategoryTags::Scalar>> *= nullptr,
              std::enable_if_t<std::is_same_v<C2, CategoryTags::Matrix>> *= nullptr>
    static auto Eval(T1&& data1, T2&& data2)
    {
        using ElementType = RawT2::ElementType;
        using DeviceType = RawT2::DeviceType;

        auto tmp_mat = MakeTrivalMatrix<ElementType, DeviceType>(
            data2.RowNum(), data2.ColNum(), std::forward<T1>(data1));

        using ResType = BinaryOp<BinaryOpTags::Substract,
                                 RemConstRef<decltype(tmp_mat)>,
                                 RawT2>;
        return ResType(std::move(tmp_mat), std::forward<T2>(data2));
    }

    template <typename C1, typename C2,
              std::enable_if_t<std::is_same_v<C1, CategoryTags::Scalar>> *= nullptr,
              std::enable_if_t<std::is_same_v<C2, CategoryTags::BatchMatrix>> *= nullptr>
    static auto Eval(T1&& data1, T2&& data2)
    {
        using ElementType = RawT2::ElementType;
        using DeviceType = RawT2::ElementType;

        auto tmp_mat = MakeTrivalMatrix<ElementType, DeviceType>(
            data2.RowNum(), data2.ColNum(), std::forward<T1>(data1));
        auto tmp_batch_mat = MakeDuplicate(data2.BatchNum(), std::move(tmp_mat));

        using ResType = BinaryOp<BinaryOpTags::Substract,
                                 RemConstRef<decltype(tmp_batch_mat)>,
                                 RawT2>;
        return ResType(std::move(tmp_batch_mat), std::forward<T2>(data2));
    }

    template <typename C1, typename C2,
              std::enable_if_t<std::is_same_v<C1, CategoryTags::BatchMatrix>> *= nullptr,
              std::enable_if_t<std::is_same_v<C2, CategoryTags::Scalar>> *= nullptr>
    static auto Eval(T1&& data1, T2&& data2)
    {
        using ElementType = RawT1::ElementType;
        using DeviceType = RawT2::DeviceType;

        auto tmp_mat = MakeTrivalMatrix<ElementType, DeviceType>(
            data1.RowNum(), data1.ColNum(), std::forward<T2>(data2));
        auto tmp_batch_mat = MakeDuplicate(data1.BatchNum(), std::move(tmp_mat));

        using ResType = BinaryOp<BinaryOpTags::Substract,
                                 RawT1,
                                 RemConstRef<decltype(tmp_batch_mat)>>;
        return ResType(std::forward<T1>(data1), std::move(tmp_batch_mat));
    }

    template <typename C1, typename C2,
              std::enable_if_t<std::is_same_v<C1, CategoryTags::Matrix>> *= nullptr,
              std::enable_if_t<std::is_same_v<C2, CategoryTags::BatchMatrix>> *= nullptr>
    static auto Eval(T1&& data1, T2&& data2)
    {
        static_assert(std::is_same_v<RawT1::ElementType, RawT2::ElementType>,
                      "Matrices with different element types cannot substract directly");
        static_assert(std::is_same_v<RawT1::DeviceType, RawT2::DeviceType>,
                      "Matrices with different device types cannot substract directly");

        auto tmp = MakeDuplicate(data2.BatchNum(), std::forward<T1>(data1));

        using ReType = BinaryOp<BinaryOpTags::Substract,
                                RemConstRef<decltype(tmp)>,
                                RawT2>;
        return ResType(std::move(tmp), std::forward<T2>(dat2));
    }

    template <typename C1, typename C2,
              std::enable_if_t<std::is_same_v<C1, CategoryTags::BatchMatrix>> *= nullptr,
              std::enable_if_t<std::is_same_v<C2, CategoryTags::Matrix>> *= nullptr>
    static auto Eval(T1&& data1, T2&& data2)
    {
        static_assert(std::is_same_v<RawT1::ElementType, RawT2::ElementType>,
                      "Matrices with different element types cannot substract directly");
        static_assert(std::is_same_v<RawT1::DeviceType, RawT2::DeviceType>,
                      "Matrices with different device types cannot substract directly");

        auto tmp = MakeDuplicate(data1.BatchNum(), std::forward<T2>(data2));

        using ResType = BinaryOp<BinaryOpTags::Substract,
                                 RawT1,
                                 RemConstRef<decltype(tmp)>>;
        return ResType(std::forward<T1>(data1), std::move(tmp));
    }
};

template <typename T1, typename T2,
          std::enable_if_t<OperSubstract_<T1, T2>::vaild>* = nullptr>
auto operator-(T1&& lhs, T2&& rhs)
{
    using Cate1 = DataCategory<T1>;
    using Cate2 = DataCategory<T2>;

    return OperSubstract_<T1, T2>::template Eval<Cate1, Cate2>(
        std::forward<T1>(lhs), std::forward<T2>(rhs));
}