#pragma once
#include "meta_nn/data/facilities/allocators.h"

template <typename Elem, typename Device>
class ContinuousMemory
{
    static_assert(std::is_same<RemConstRef<Elem>, Elem>::value);

public:
    explict ContinuousMemory(size_t size)
        : mem(Allocator<Device>::template Allocate<Elem>(size))
        , mem_start(mem.get())
    {
    }

    ContinuousMemory(std::shared_ptr<Elem> mem_, Elem* mem_start)
        : mem(std::move(mem_))
        , mem_start(mem_start)
    {
    }

    auto RawMemory() const { return mem_start; }

    const auto SharedPtr() const { return mem; }

    size_t UseCount() const { return mem.use_count(); }

    bool operator==(const ContinuousMemory& other) const
    {
        return mem == other.mem && mem_start == other.mem;
    }

    bool operator!=(const ContinuousMemory& other) const
    {
        return !(operator==(other));
    }

private:
    std::shared_ptr<Elem> mem;
    Elem* mem_start;
};