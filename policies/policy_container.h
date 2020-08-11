#pragma once

template <typename... Policies>
struct PolicyContainer;

template <typename T>
constexpr bool IsPolicyContainer = false;

template <typename... T>
constexpr bool IsPolicyContainer<PolicyContainer<T...>> = true;

template <typename LayerName, typename... Policies>
struct SubPolicyContainer;

template <typename T>
constexpr bool IsSubPolicyContainer = false;

template <typename LayerName, typename... T>
constexpr bool IsSubPolicyContainer<SubPolicyContainer<LayerName, T...>> = true;