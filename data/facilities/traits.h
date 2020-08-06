#pragma once
#include "meta_nn/data/facilities/tags.h"

////////////////////// Type Define ////////////////////////
template <typename Elem, typename Device>
class Matrix;

template <typename Elem, typename Device>
class Scalar;

template <typename Elem, typename Device, typename Category>
class Batch;

////////////////////// PrincipalDataType ////////////////////////
template <typename Category, typename Elem, typename Device>
struct PrincipalDataType_;

template <typename Elem, typename Device>
struct PrincipalDataType_<CategoryTags::Matrix, Elem, Device>
{
    using type = Matrix<Elem, Device>;
};

template <typename Elem, typename Device>
struct PrincipalDataType_<CategoryTags::Scalar, Elem, Device>
{
    using type = Scalar<Elem, Device>;
};

template <typename Elem, typename Device>
struct PrincipalDataType_<CategoryTags::BatchMatrix, Elem, Device>
{
    using type = Batch<Elem, Device, CategoryTags::Matrix>;
};

template <typename Elem, typename Device>
struct PrincipalDataType_<CategoryTags::BatchScalar, Elem, Device>
{
    using type = Batch<Elem, Device, CategoryTags::Scalar>;
};

template <typename Category, typename Elem, typename Device>
using PrincipalDataType = typename PrincipalDataType_<Ctaegory, Elem, Device>::type;

////////////////////// IsScalar ////////////////////////
template <typename T>
constexpr bool IsScalar = false;

template <typename T>
constexpr bool IsScalar<const T> = IsScalar<T>;

template <typename T>
constexpr bool IsScalar<T&> = IsScalar<T>;

template <typename T>
constexpr bool IsScalar<T&&> = IsScalar<T>;

////////////////////// IsMatrix ////////////////////////
template <typename T>
constexpr bool IsMatrix = false;

template <typename T>
constexpr bool IsMatrix<const T> = IsMatrix<T>;

template <typename T>
constexpr bool IsMatrix<T&> = IsMatrix<T>;

template <typename T>
constexpr bool IsMatrix<T&&> = IsMatrix<T>;

////////////////////// IsBatchScalar ////////////////////////
template <typename T>
constexpr bool IsBatchScalar = false;

template <typename T>
constexpr bool IsBatchScalar<const T> = IsBatchScalar<T>;

template <typename T>
constexpr bool IsBatchScalar<T&> = IsBatchScalar<T>;

template <typename T>
constexpr bool IsBatchScalar<const T&> = IsBatchScalar<T>;

template <typename T>
constexpr bool IsBatchScalar<T&&> = IsBatchScalar<T>;

template <typename T>
constexpr bool IsBatchScalar<const T&&> = IsBatchScalar<T>;

////////////////////// IsBatchMatrix ////////////////////////
template <typename T>
constexpr bool IsBatchMatrix = false;

template <typename T>
constexpr bool IsBatchMatrix<const T> = IsBatchMatrix<T>;

template <typename T>
constexpr bool IsBatchMatrix<T&> = IsBatchMatrix<T>;

template <typename T>
constexpr bool IsBatchMatrix<const T&> = IsBatchMatrix<T>;

template <typename T>
constexpr bool IsBatchMatrix<T&&> = IsBatchMatrix<T>;

template <typename T>
constexpr bool IsBatchMatrix<const T&&> = IsBatchMatrix<T>;

////////////////////// DataCategory ////////////////////////
template <typename T>
struct DataCategory_
{
private:
    template <bool is_scalar, bool is_matrix,
              bool is_batch_scalar, bool is_batch_matrix,
              typename Dummy = void>
    struct Helper;

    template <typename Dummy>
    struct Helper<true, false, false, false, Dummy>
    {
        using type = CategoryTags::Scalar;
    };

    template <typename Dummy>
    struct Helper<false, true, false, false, Dummy>
    {
        using type = CategoryTags::Matrix;
    };

    template <typename Dummy>
    struct Helper<false, false, true, false, Dummy>
    {
        using type = CategoryTags::BatchScalar;
    };

    template <typename Dummy>
    struct Helper<false, false, false, true, Dummy>
    {
        using type = CategoryTags::BatchMatrix;
    };

public:
    using type = typename Helper<IsScalar<T>,
                                 IsMatrix<T>,
                                 IsBatchScalar<T>,
                                 IsBatchMatrix<T>>::type;
};

template <typename T>
using DataCategory = typename DataCategory_<T>::type;
