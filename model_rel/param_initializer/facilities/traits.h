#pragma once
#include "meta_nn/facilities/traits.h"
#include "meta_nn/facilities/var_type_dict.h"
#include "meta_nn/model_rel/param_initializer/facilities/policies.h"
#include "meta_nn/policies/policy_container.h"

namespace NSParamInit
{
//////////////////////// FillerTagFromPolicy_ //////////////////////////
template <typename Res, typename... Policies>
struct FillerTagFromPolicy_
{
    using type = Res;
};

template <typename... Res, typename Cur, typename... Rem>
struct FillerTagFromPolicy_<std::tuple<Res...>, PInitializerIs<Cur>, Rem...>
{
    using type = typename FillerTagFromPolicy_<std::tuple<Res..., Cur>, Rem...>::type;
};

template <typename... Res, typename Cur, typename... Rem>
struct FillerTagFromPolicy_<std::tuple<Res...>, PWeightInitializerIs<Cur>, Rem...>
{
    using type = typename FillerTagFromPolicy_<std::tuple<Res..., Cur>, Rem...>::type;
};

template <typename... Res, typename Cur, typename... Rem>
struct FillerTagFromPolicy_<std::tuple<Res...>, PBiasInitializerIs<Cur>, Rem...>
{
    using type = typename FillerTagFromPolicy_<std::tuple<Res..., Cur>, Rem...>::type;
};

template <typename... Res, typename Layer, typename... Sub, typename... Rem>
struct FillerTagFromPolicy_<std::tuple<Res...>, SubPolicyContainer<Layer, Sub...>, Rem...>
{
    using step1 = typename FillerTagFromPolicy_<std::tuple<Res...>, Sub...>::type;
    using type = typename FillerTagFromPolicy_<step1, Rem...>::type;
};

//////////////////////// AlreadyExist_ //////////////////////////
template <typename Checker, typename... Checkes>
struct AlreadyExist_
{
    constexpr static bool value = false;
};

template <typename Checker, typename Cur, typename... Rem>
struct AlreadyExist_<Checker, Cur, Rem...>
{
    constexpr static bool value = OrValue<std::is_same_v<Checker, Cur>,
                                          AlreadyExist_<Checker, Rem...>>;
};

//////////////////////// TagDedupe_ //////////////////////////
template <typename Res, typename PolicyTuple>
struct TagDedupe_
{
    using type = Res;
};

template <typename... Res, typename Cur, typename... Rem>
struct TagDedupe_<VarTypeDict<Res...>, std::tuple<Cur, Rem...>>
{
    using type = typename std::conditional_t<AlreadyExist_<Cur, Res...>::value,
                                             TagDedupe_<VarTypeDict<Res...>, std::tuple<Rem...>>,
                                             TagDedupe_<VarTypeDict<Res..., Cur>, std::tuple<Rem...>>>::type;
};

//////////////////////// FillerTags2NamedParams_ //////////////////////////
template <typename... Policies>
struct FillerTags2NamedParams_
{
    using step1 = typename FillerTagFromPolicy_<std::tuple<>, Policies...>::type;
    using type = typename TagDedupe_<VarTypeDict<>, step1>::type;
};

} // namespace NSParamInit

//////////////////////// FillerTags2NamedParams ////////////////////////////////
template <typename... Policies>
using FillerTags2NamedParams = typename NSParamInit::FillerTags2NamedParams_<Policies...>::type;