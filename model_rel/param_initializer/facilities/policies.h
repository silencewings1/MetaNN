#pragma once
#include "meta_nn/policies/policy_macro_begin.h"
#include <random>

struct InitPolicy
{
    using MajorClass = InitPolicy;

    struct OverallTypeCate;
    struct WeightTypeCate;
    struct BisaTypeCate;

    using Overall = void;
    using Weight = void;
    using Bias = void;

    struct RandEngineTypeCate;
    using RandEngine = std::mt19937;
};

TypePolicyTemplate(PInitializerIs, InitPolicy, Overall);
TypePolicyTemplate(PWeightInitializerIs, InitPolicy, Weight);
TypePolicyTemplate(PBiasInitializerIs, InitPolicy, Bias);
TypePolicyTemplate(PRandomGeneratorIs, InitPolicy, RandEngine);

// TODO

#include "meta_nn/policies/policy_macro_end.h"