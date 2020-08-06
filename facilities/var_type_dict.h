#pragma once
#include <iostream>
#include <memory>

namespace NSMultiTypeDict
{

//////////////////////////// NullPara /////////////////////////////////
struct NullPara
{
};

//////////////////////////// Identity /////////////////////////////////
template <typename T>
struct Identity_
{
    using type = T;
};

//////////////////////////// Create /////////////////////////////////
template <size_t N, template <typename...> class Container, typename... T>
struct Create_
{
    using type = typename Create_<N - 1, Container, NullPara, T...>::type;
};

template <template <typename...> class Container, typename... T>
struct Create_<0, Container, T...>
{
    using type = Container<T...>;
};

//////////////////////////// Tag2ID /////////////////////////////////
template <typename TarTag, size_t N, typename CurTag, typename... Tags>
struct Tag2ID_
{
    constexpr static size_t value = Tag2ID_<TarTag, N + 1, Tags...>::value;
};

template <typename TarTag, size_t N, typename... Tags>
struct Tag2ID_<TarTag, N, TarTag, Tags...>
{
    constexpr static size_t value = N;
};

template <typename TarTag, typename... Tags>
constexpr size_t Tag2ID = Tag2ID_<TarTag, 0, Tags...>::value;

//////////////////////////// NewTupleType /////////////////////////////////
template <typename Val, size_t N, size_t M,
          typename ProcessedTypes, typename... RemainTypes>
struct NewTupleType_;

template <typename Val, size_t N, size_t M,
          template <typename...> class Container,
          typename... ModifiedTypes,
          typename CurType,
          typename... RemainTypes>
struct NewTupleType_<Val, N, M,
                     Container<ModifiedTypes...>,
                     CurType, RemainTypes...>
{
    using type = typename NewTupleType_<Val, N, M + 1,
                                        Container<ModifiedTypes..., CurType>,
                                        RemainTypes...>::type;
};

template <typename Val, size_t N,
          template <typename...> class Container,
          typename... ModifiedTypes,
          typename CurType,
          typename... RemainTypes>
struct NewTupleType_<Val, N, N,
                     Container<ModifiedTypes...>,
                     CurType, RemainTypes...>
{
    using type = Container<ModifiedTypes..., Val, RemainTypes...>;
};

template <typename Val, size_t Pos, typename Container, typename... RemainTypes>
using NewTupleType = typename NewTupleType_<Val, Pos, 0, Container, RemainTypes...>::type;

//////////////////////////// Pos2Type /////////////////////////////////

template <size_t Pos, typename... Tags>
struct Pos2Type_
{
    static_assert((Pos != 0), "Invaild position.");
};

template <size_t Pos, typename Cur, typename... Tags>
struct Pos2Type_<Pos, Cur, Tags...>
{
    using type = typename std::conditional_t<(Pos == 0),
                                             Identity_<Cur>,
                                             Pos2Type_<Pos - 1, Tags...>>::type;
};

template <size_t Pos, typename... Tags>
using Pos2Type = typename Pos2Type_<Pos, Tags...>::type;

} // namespace NSMultiTypeDict

template <typename... Paras>
struct VarTypeDict
{
    template <typename... Types>
    struct Values
    {
        Values() = default;

        Values(std::shared_ptr<void>(&&input)[sizeof...(Types)])
        // Values(std::shared_ptr<void>[sizeof...(Types)] && input)
        {
            for (size_t i = 0; i < sizeof...(Types); ++i)
            {
                m_tuple[i] = std::move(input[i]);
            }
        }

        template <typename Tag, typename Val>
        auto Set(Val&& val) &&
        {
            using namespace NSMultiTypeDict;
            constexpr static size_t TagPos = Tag2ID<Tag, Paras...>;

            using RawVal = std::decay_t<Val>;
            auto tmp = new RawVal(std::forward<Val>(val));
            m_tuple[TagPos] = std::shared_ptr<void>(tmp,
                                                  [](void* ptr) { auto nptr = static_cast<RawVal*>(ptr); delete nptr; });

            using NewType = NewTupleType<RawVal, TagPos, Values<>, Types...>;
            return NewType(std::move(m_tuple));
        }

        template <typename Tag>
        const auto& Get() const
        {
            using namespace NSMultiTypeDict;
            constexpr static size_t TagPos = Tag2ID<Tag, Paras...>;

            using AimType = Pos2Type<TagPos, Types...>;
            auto tmp = m_tuple[TagPos].get();
            auto res = static_cast<AimType*>(tmp);

            return *res;
        }

    private:
        std::shared_ptr<void> m_tuple[sizeof...(Types)];
    };

    static auto Create()
    {
        using namespace NSMultiTypeDict;
        using type = typename Create_<sizeof...(Paras), Values>::type;

        return type();
    }
};