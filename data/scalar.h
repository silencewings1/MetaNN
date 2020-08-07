#pragma once
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/facilities/traits.h"
//#include "meta_nn/evaluate/facilities/eval_handle.h"

template <typename Elem, typename Device = DeviceTags::CPU>
class Scalar
{
public:
    using ElementType = Elem;
    using DeviceType = Device;

public:
    Scalar(ElementType elem = ElementType())
        : elem(elem)
    {
    }

    auto& Value() { return elem; }

    auto Value() const { return elem; }

    bool operator==(const Scalar& other) const { return elem == other.elem; }

    template <typename OtherType>
    bool operator==(const OtherType&) const { return false; }

    template <typename T>
    bool operator==(const T& other) const { return !(operator==(other)); }

    // TODO 
    // auto EvalRegister() const { return MakeConstEvalHandle(*this); }

private:
    ElementType elem;
};

template <typename Elem, typename Device>
constexpr bool IsScalar<Scalar<Elem, Device>> = true;