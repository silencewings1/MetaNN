#pragma once

template <typename OpTag, typename Op1, typename... Operands>
struct OperElementType_
{
    using type = typename Op1::ElementType;
};

template <typename OpTag, typename Op1, typename... Operands>
struct OperDeviceType_
{
    using type = typename Op1::DeviceType;
};

template <typename OpTag, typename Op1, typename... Operands>
using OperElementType = typename OperElementType_<OpTag, Op1, Operands...>::type;

template <typename OpTag, typename Op1, typename... Operands>
using OperDeviceType = typename OperDeviceType_<OpTag, Op1, Operands...>::type;