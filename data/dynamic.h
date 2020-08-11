#pragma once
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/scalar.h"
#include "meta_nn/facilities/traits.h"
// #include <MetaNN/evaluate/facilities/eval_buffer.h>
#include <memory>

////////////////////////// DynamicCategory ///////////////////////////////
template <typename Elem, typename Device, typename DataCate>
class DynamicCategory;

template <typename Elem, typename Device>
class DynamicCategory<Elem, Device, CategoryTags::Matrix>
{
public:
    using ElementType = Elem;
    using DeviceType = Device;
    using EvalType = PrincipalDataType<CategoryTags::Matrix, ElementType, DeviceType>;

public:
    template <typename Base>
    DynamicCategory(const Base& base)
        : row_num(base.RowNum())
        , col_num(base.ColNum())
    {
    }

    virtual ~DynamicCategory() = default;

    virtual bool operator==(const DynamicCategory& other) const = 0;
    virtual bool operator!=(const DynamicCategory& other) const = 0;

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }

    // TODO: Eval
    //  virtual DynamicConstEvalHandle<EvalType> EvalRegister() const = 0;

private:
    size_t row_num;
    size_t col_num;
};

template <typename Elem, typename Device>
class DynamicCategory<Elem, Device, CategoryTags::BatchMatrix>
{
public:
    using ElementType = Elem;
    using DeviceType = Device;
    using EvalType = PrincipalDataType<CategoryTags::BatchMatrix, ElementType, DeviceType>;

public:
    template <typename Base>
    DynamicCategory(const Base& base)
        : row_num(base.RowNum())
        , col_num(base.ColNum())
        , batch_num(base.BatchNum())
    {
    }

    virtual ~DynamicCategory() = default;

    virtual bool operator==(const DynamicCategory& other) const = 0;
    virtual bool operator!=(const DynamicCategory& other) const = 0;

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }
    size_t BatchNum() const { return batch_num; }

    // TODO: Eval
    //  virtual DynamicConstEvalHandle<EvalType> EvalRegister() const = 0;

private:
    size_t row_num;
    size_t col_num;
    size_t batch_num;
};

////////////////////////// DynamicWrapper ///////////////////////////////
template <typename TBaseData>
class DynamicWrapper : public DynamicCategory<typename TBaseData::ElementType,
                                              typename TBaseData::DeviceType,
                                              DataCategory<TBaseData>>
{
    using Base = DynamicCategory<typename TBaseData::ElementType,
                                 typename TBaseData::DeviceType,
                                 DataCategory<TBaseData>>;

public:
    DynamicWrapper(TBaseData data_)
        : Base(data_)
        , data(std::move(data_))
    {
    }

    const TBaseData& BaseData() const { return data; }

    bool operator==(const Base& other) const override
    {
        try
        {
            const auto& real = dynamic_cast<const DynamicWrapper&>(other);
            return data == real.data;
        }
        catch (std::bad_cast&)
        {
            return false;
        }
    }

    bool operator!=(const Base& other) const override
    {
        return !(operator==(other));
    }

    // TODO: Eval
    // DynamicConstEvalHandle<EvalType> EvalRegister() const override;

private:
    TBaseData data;
};

////////////////////////// DynamicData ///////////////////////////////
template <typename Elem, typename Device, typename DataCate>
class DynamicData;

template <typename Elem, typename Device>
class DynamicData<Elem, Device, CategoryTags::Matrix>
{
    using BaseData = DynamicCategory<Elem, Device, CategoryTags::Matrix>;

public:
    using ElementType = Elem;
    using DeviceType = Device;
    //using ResHandleType = dec

public:
    DynamicData() = default;

    template <typename OriginType>
    DynamicData(std::shared_ptr<DynamicWrapper<OriginType>> data_)
        : data(std::move(data_))
    {
    }

    size_t RowNum() const { return data->RowNum(); }
    size_t ColNum() const { return data->ColNum(); }

    // TODO: EVAL

    bool operator==(const DynamicData& other) const
    {
        if (!data && !other.data)
            return true;
        if (!data || !other.data)
            return false;

        BaseData& val1 = *data;
        BaseData& val2 = *(other.data);
        return val1 == val2;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(cosnt T& other) const { return !(operator==(other)); }

    template <typename T>
    const T& TypeCast() const
    {
        const BaseData* ptr = data.get();
        auto ptr_cast = dynamic_cast<const DynamicWrapper<T>*>(ptr);

        return ptr_cast ? ptr_cast->BaseData() : nullptr;
    }

    bool IsEmpty() const
    {
        return data == nullptr;
    }

private:
    std::shared_ptr<BaseData> data;
};

template <typename Elem, typename Device>
class DynamicData<Elem, Device, CategoryTags::BatchMatrix>
{
    using BaseData = DynamicCategory<Elem, Device, CategoryTags::BatchMatrix>;

public:
    using ElementType = Elem;
    using DeviceType = Device;
    //using ResHandleType = dec

public:
    DynamicData() = default;

    template <typename OriginType>
    DynamicData(std::shared_ptr<DynamicWrapper<OriginType>> data_)
        : data(std::move(data_))
    {
    }

    size_t RowNum() const { return data->RowNum(); }
    size_t ColNum() const { return data->ColNum(); }
    size_t BatchNum() const { return data->BatchNum(); }

    // TODO: EVAL

    bool operator==(const DynamicData& other) const
    {
        if (!data && !other.data)
            return true;
        if (!data || !other.data)
            return false;

        BaseData& val1 = *data;
        BaseData& val2 = *(other.data);
        return val1 == val2;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(cosnt T& other) const { return !(operator==(other)); }

    template <typename T>
    const T& TypeCast() const
    {
        const BaseData* ptr = data.get();
        auto ptr_cast = dynamic_cast<const DynamicWrapper<T>*>(ptr);

        return ptr_cast ? ptr_cast->BaseData() : nullptr;
    }

    bool IsEmpty() const
    {
        return data == nullptr;
    }

private:
    std::shared_ptr<BaseData> data;
};

////////////////////////// meta ///////////////////////////////
template <typename Elem, typename Device>
constexpr bool IsMatrix<DynamicData<Elem, Device, CategoryTags::Matrix>> = true;

template <typename Elem, typename Device>
constexpr bool IsBatchMatrix<DynamicData<Elem, Device, CategoryTags::BatchMatrix>> = true;

template <typename Data>
constexpr bool IsDynamic = false;

template <typename Elem, typename Device, typename Cate>
constexpr bool IsDynamic<DynamicData<Elem, Device, Cate>> = true;

template <typename Elem, typename Device, typename Cate>
constexpr bool IsDynamic<DynamicData<Elem, Device, Cate>&> = true;

template <typename Elem, typename Device, typename Cate>
constexpr bool IsDynamic<DynamicData<Elem, Device, Cate>&&> = true;

template <typename Elem, typename Device, typename Cate>
constexpr bool IsDynamic<const DynamicData<Elem, Device, Cate>&> = true;

template <typename Elem, typename Device, typename Cate>
constexpr bool IsDynamic<const DynamicData<Elem, Device, Cate>&&> = true;

////////////////////////// make ///////////////////////////////
template <typename Data>
auto MakeDynamic(Data&& data)
{
    if constexpr (IsDynamic<Data>)
    {
        return std::forward<Data>(data);
    }
    else
    {
        using RawData = RemConstRef<Data>;
        using DeriveData = DynamicWrapper<RawData>;

        auto base_data = std::make_shared<DeriveData>(std::forward<Data>(data));
        return DynamicData<typename RawData::ElementType,
                           typename RawData::DeviceType,
                           DataCategory<RawData>>(std::move(base_data));
    }
}
