#pragma once

template <typename Data>
struct LowerAccessImpl;

template <typename Data>
auto LowerAccess(Data&& p)
{
    using RawType = RemConstRef<Data>;
    return LowerAccessImpl<RawType>(std::forward<Data>(p));
}