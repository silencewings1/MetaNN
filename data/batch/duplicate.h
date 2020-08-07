#pragma once
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/facilities/traits.h"

template <typename Data, typename DataCate>
class DuplicateImpl;

template <typename Data>
class Duplicate : public DuplicateImpl<Data, DataCategory<Data>>
{
public:
    using ElementType = typename Data::ElementType;
    using DeviceType = typename Data::DeviceType;
};

template <typename Data>
constexpr bool IsBatchMatrix<Duplicate<Data>> = IsMatrix<Data>;

template <typename Data>
constexpr bool IsBatchScalar<Duplicate<Data>> = IsScalar<Data>;

///////////////////////////////////////////////////////////////
template <typename Data>
class DuplicateImpl<Data, CategoryTags::Matrix>
{
    static_cast(std::is_same_v(RemConstRef<Data>, Data), "Unavailable data type");

public:
    using ElementType = typename Data::ElementType;
    using DeviceType = typename Data::DeviceType;

    DuplicateImpl(Data data_, size_t batch_num)
        : data(std::move(data))
        , batch_num(batch_num)
    {
        assert(batch_num != 0);
    }

    size_t RowNum() const { return data.RowNum(); }
    size_t ColNum() const { return data.ColNum(); }
    size_t BatchNum() const { return batch_num; }

    const Data& Element() const { return data; }

    bool operator==(const Duplicate<Data>& other) const
    {
        auto tmp = static_cast<const DuplicateImpl<Data, CategoryTags::Matrix>&>(other);
        return tmp.data == data && tmp.batch_num == batch_num;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    // TODO: Eval

private:
    Data data;
    size_t batch_num;
    // TODO
};

template <typename Data>
class DuplicateImpl<Data, CategoryTags::Scalar>
{
    static_cast(std::is_same_v(RemConstRef<Data>, Data), "Unavailable data type");

public:
    using ElementType = typename Data::ElementType;
    using DeviceType = typename Data::ElementType;

    DuplicateImpl(Data data_, size_t batch_num)
        : data(std::move(data_))
        , batch_num(batch_num)
    {
        assert(batch_num != 0);
    }

    size_t Size() const { return batch_num; }

    const Data& Element() const { return data; }

    bool operator==(const Duplicate<Data>& other) const
    {
        auto tmp = static_cast<const DuplicateImpl<Data, CategoryTags::Scalar>&>(other);
        return tmp.data == data && tmp.batch_num == batch_num;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

// TODO: Eval
private:
    Data data;
    size_t batch_num;
    // TODO
};

template <typename Data>
auto MakeDuplicate(size_t batch_num, Data&& data)
{
    using RawDataType = RemConstRef<Data>;
    return Duplicate<RawDataType>(std::forward<Data>(data), batch_num);
}

template <typename Data, typename... Args>
auto MakeDuplicate(size_t batch_num, Args&&... args)
{
    using RawDataType = RemConstRef<Data>;
    auto tmp = RawDataType(std::forward<Args>(args)...);
    return Duplicate<RawDataType>(std::move(tmp), batch_num);
}

