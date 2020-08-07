#pragma once
#include "meta_nn/data/batch/batch.h"
#include <stdexcept>
#include <vector>

template <typename Data, typename DataCate>
class ArrayImpl;

template <typename Data>
class Array : public ArrayImpl<Data, DataCategory<Data>>
{
    using ElementType = typename Data::ElementType;
    using DeviceType = typename Data::DeviceType;
    using ArrayImpl<Data, DataCategory<Data>>::ArrayImpl;
};

template <typename Data>
constexpr bool IsBatchMatrix<Array<Data>> = IsMatrix<Data>;

template <typename Data>
constexpr bool IsBatchScalar<Array<Data>> = IsScalar<Data>;

//////////////////////////////////////////////////////////
template <typename Data>
class ArrayImpl<Data, CategoryTags::Matrix>
{
public:
    using ElementType = typename Data::ElementType;
    using DeviceType = typename Data::DeviceType;

public:
    ArrayImpl(size_t row_num, size_t col_num)
        : row_num(row_num)
        , col_num(col_num)
        , buffer(std::make_shared<std::vector<Data>>())
    {
    }

    template <typename Iterator, std::enable_if_t<IsIterator<Iterator>> *= nullptr>
    ArrayImpl(Iterator begin, Iterator end)
        : row_num(0)
        , col_num(0)
        , buffer(std::make_shared<std::vector<Data>>(begin, end))
    {
        const auto& buf = *buffer;
        if (!buf.empty())
        {
            row_num = buf.front().RowNum();
            col_num = buf.front().ColNum();

            for (size_t i = 1; i < buf.size(); ++i)
            {
                if (buf[i].RowNum() != row_num ||
                    buf[i].ColNum() != col_num)
                {
                    throw std::runtime_error("Dimension mismatch");
                }
            }
        }
    }

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }
    size_t BatchNum() const { return buffer.size(); }

    size_t size() const { return BatchNum(); }

    void push_back(Data mat)
    {
        assert(AvailableForWrite());
        if (mat.RowNum() != row_num || mat.ColNum() != col_num)
        {
            throw std::runtime_error("Dimension mismatch");
        }

        buffer->emplace_back(std::move(mat));
    }

    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        assert(AvailableForWrite());
        auto mat = Data(std::forward<Args>(args)...);
        if (mat.RowNum() != row_num || mat.ColNum() != col_num)
        {
            throw std::runtime_error("Dimension mismatch");
        }

        buffer->emplace_back(std::move(mat));
    }

    void reserve(size_t num)
    {
        assert(AvailableForWrite());
        buffer->reserve(num);
    }

    void clear()
    {
        assert(AvailableForWrite());
        buffer->clear();
    }

    bool empty() const
    {
        assert(AvailableForWrite());
        buffer->empty();
    }

    auto& operator[](size_t id) { return (*buffer)[id]; }
    const auto& operator[](size_t id) const { return (*buffer)[id]; }

    auto begin() { return buffer->begin(); }
    auto begin() const { return buffer->begin(); }
    auto end() { return buffer->end(); }
    auto end() const { return buffer->end(); }

    bool operator==(const Array<Data>& other) const
    {
        auto tmp = static_cast<const ArrayImpl<Data, CategoryTags::Matrix>&>(other);
        return tmp.buffer == buffer;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    bool AvailableForWrite() const
    {
        // TODO
        return buffer.use_count() == 1;
    }

    // TODO: Eval

protected:
    size_t row_num;
    size_t col_num;
    std::shared_ptr<std::vector<Data>> buffer;
};

template <typename Data>
class ArrayImpl<Data, CategoryTags::Scalar>
{
public:
    using ElementType = typename Data::ElementType;
    using DeviceType = typename Data::DeviceType;

public:
    ArrayImpl(size_t row_num, size_t col_num)
        : buffer(std::make_shared<std::vector<Data>>())
    {
    }

    template <typename Iterator, std::enable_if_t<IsIterator<Iterator>>* = nullptr>
    ArrayImpl(Iterator begin, Iterator end)
        : buffer(std::make_shared<std::vector<Data>>(begin, end))
    {
    }

    size_t BatchNum() const { return buffer->size(); }
    size_t size() const { return buffer->size(); }
    bool empty() const { return buffer->empty(); }

    void push_back(Data val)
    {
        assert(AvailableForWrite());
        buffer->emplace_back(std::move(val));
    }

    template <typename... Args>
    void emplace_back(Args... args)
    {
        assert(AvailableForWrite());
        auto val = Data(std::forward<Args>(args)...);
        buffer->emplace_back(std::move(val));
    }

    void reserve(size_t num)
    {
        assert(AvailableForWrite());
        buffer->reserve(num);
    }

    void clear()
    {
        assert(AvailableForWrite());
        buffer->clear();
    }

    auto& operator[](size_t id) { return (*buffer)[id]; }
    const auto& operator[](size_t id) const { return (*buffer)[id]; }

    auto begin() { return buffer->begin(); }
    auto begin() const { return buffer->begin(); }
    auto end() { return buffer->end(); }
    auto end() const { return buffer->end(); }

    bool operator==(const Array<Data>& other) const
    {
        auto val = static_cast<const ArrayImpl<Data, CategoryTags::Scalar>&>(other);
        return buffer == val.buffer;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    bool AvailableForWrite() const
    {
        // TODO
        return buffer.use_count() == 1;
    }

    // TODO: Eval

protected:
    std::shared_ptr<std::vector<Data>> buffer;
    // TODO
};

template <typename Iterator>
auto MakeArray(Iterator begin, Iterator end)
{
    using Data = typename std::iterator_traits<Iterator>::value_type;
    using RawData = RemConstRef<Data>;

    return Array<RawData>(begin, end);
}

