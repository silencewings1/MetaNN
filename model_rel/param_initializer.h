#pragma once
#include "meta_nn/data/matrices/cpu_matrix.h"
#include "meta_nn/data_copy/data_copy.h"
#include "meta_nn/model_rel/param_initializer/facilities/policies.h"
#include "meta_nn/model_rel/param_initializer/facilities/traits.h"
#include "meta_nn/policies/policy_container.h"
#include <map>
#include <stdexcept>
#include <string>

template <typename Elem, typename PolicyCont, typename Fillers>
class ParamInitializer
{
    using ElementType = Elem;
    using DeviceType = DeviceTags::CPU;
    using MatrixType = Matrix<ElementType, DeviceType>;

public:
    ParamInitializer(Fillers&& filler_)
        : filler(std::move(filler_))
    {
    }

    template <typename Tag, typename Val>
    auto SetFiller(Val&& val) &&
    {
        auto new_filler = std::move(filler).template Set<Tag, Val>(std::forward<Val>(val));
        using NewFillerType = RemConstRef<decltype(new_filler)>;
        return ParamInitializer<Elem, PolicyCont, NewFillerType>(std::move(new_filler));
    }

    template <typename Tag>
    auto& GetFiller()
    {
        return filler.template Get<Tag>();
    }

    template <typename Elem2, typename Device2>
    void SetMatrix(const std::string& name, const Matrix<Elem2, Device2>& mat)
    {
        if (params.find(name) != params.end())
        {
            throw std::runtime_error("Duplicate parameter matrix: " + name);
        }

        if constexpr (std::is_same_v<ElementType, Elem2> &&
                      std::is_same_v<DeviceType, Device2>)
        {
            params.insert({name, mat});
        }
        else
        {
            MatrixType new_mat(mat.RowNum(), mat.ColNum());
            DataCopy(mat, new_mat);
            params.insert({name, std::move(new_mat)});
        }
    }

    template <typename Elem2, typename Device2>
    void GetMatrix(const std::string& name, Matrix<Elem2, Device2>& mat)
    {
        auto it = params.find(name);
        if (it == params.end())
        {
            throw std::runtime_error("Parameter not exist: " + name);
        }

        const auto& origin_mat = it->second;
        if (origin_mat.RowNum() != mat.RowNum() || origin_mat.ColNum() != mat.ColNum())
        {
            throw std::runtime_error("Matrices with different dimensions.");
        }

        DataCopy(origin_mat, mat);
    }

    bool IsMatrixExist(const std::string& name) const
    {
        return params.find(name) != params.end();
    }

private:
    Fillers filler;
    std::map<std::string, MatrixType> params;
};

template <typename Elem, typename... Policies>
auto MakeInitializer()
{
    using npType = FillerTags2NamedParams<Policies...>;
    using FilDictType = RemConstRef<decltype(npType::Create())>;
    return ParamInitializer<Elem, PolicyContainer<Policies...>, FilDictType>(npType::Create());
}