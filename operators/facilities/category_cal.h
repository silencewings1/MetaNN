#pragma once
#include "meta_nn/data/facilities/traits.h"
#include "meta_nn/facilities/traits.h"
#include <tuple>

/////////////////////////// Data2Cate //////////////////////////
template <typename CateCont, typename... Data>
struct Data2Cate_
{
    using type = CateCont;
};

template <typename... Processed, typename Cur, typename... Remain>
struct Data2Cate_<std::tuple<Processed...>, Cur, Remain...>
{
    using tmp1 = DataCategory<Cur>;
    using tmp2 = std::tuple<Processed, tmp1>;
    using type = typename Data2Cate_<tmp2, Remain...>::type;
};

template <typename Head, typename... Remain>
using Data2Cate = typename Data2Cate_<Head, Remain...>::type;

/////////////////////////// SameCate //////////////////////////
template <typename Cate, typename... Remain>
struct SameCate_
{
    constexpr static bool value = true;
};

template <typename Cate, typename Cur, typename... Remain>
struct SameCate_<Cate, Cur, Remain...>
{
    constexpr static bool tmp = std::is_same_v<Cate, Cur>;
    constexpr static bool value = AndValue<tmp, SameCate_<Cate, Remain...>>;
};

/////////////////////////// CateInduce //////////////////////////
template <typename OpTag, typename HeadCate, typename... RemainCates>
struct OperCategory_
{
    static_assert(SameCate_<HeadCate, RemainCates...>::value,
                  "Data category mismatch.");
    using type = HeadCate;
};

template <typename OpTag, typename CateContainer>
struct CateInduce_;

template <typename OpTag, typename... Cates>
struct CateInduce_<OpTag, std::tuple<Cates...>>
{
    using type = typename OperCategory_<OpTag, Cates...>::type;
};

/////////////////////////// OperCateCal //////////////////////////
template <typename OpTag, typename Head, typename... Remain>
using OperCateCal = typename CateInduce_<OpTag,
                                         Data2Cate_<Head, Remain...>>::type;