#pragma once
#include "meta_nn/data/batch/batch.h"

template <typename Elem>
struct LowerAccessImpl<Batch<Elem, DeviceTags::CPU, CategoryTags::Scalar>>;

template <typename Elem>
class Batch<Elem, DeviceTags::CPU, CategoryTags::Scalar>
{
public:
    using ElementType = Elem;
    using DeviceType = DeviceTags::CPU;

    friend LowerAccessImpl<Batch<Elem, DeviceTags::CPU, CategoryTags::Scalar>>;

public:
    Batch(size_t length = 0)
        : mem(length)
        , len(length)
    {
    }

    size_t BatchNum() const { return len; }

    bool AvailableForWrite() const { return mem.UseCount() == 1; }

    void SetValue(size_t id, ElementType val)
    {
        assert(AvailableForWrite());
        assert(id < len);

        (mem.RawMemory())[id] = val;
    }

    const auto operator[](size_t id) const
    {
        assert(id < len);
        return (mem.RawMemory())[id];
    }

    bool operator==(const Batch& other) const
    {
        return mem == other.mem &&
               len == other.len;
    }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator!=(const T& other) const { return !(operator==(other)); }

    // TODO: Eval

private:
    ContinuousMemory<ElementType, DeviceType> mem;
    size_t len;
};

template <typename Elem>
struct LowerAccessImpl<Batch<Elem, DeviceTags::CPU, CategoryTags::Scalar>>
{
private:
    using BatchType = Batch<Elem, DeviceTags::CPU, CategoryTags::Scalar>;

public:
    LowerAccessImpl(BatchType p)
        : data(std::move(p))
    {
    }

    auto MutableRawMemory() { return data.mem.RawMemory(); }
    const auto RawMemory() const { return data.mem.RawMemory(); }

private:
    BatchType data;
};