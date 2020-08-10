#pragma once
#include "meta_nn/data/facilities/tags.h"
#include "meta_nn/data/facilities/traits.h"

template <typename OpTag, typename Cate>
class OperOrganizer;

template <typename OpTag>
class OperOrganizer<OpTag, CategoryTags::Scalar>
{
public:
    template <typename Head, typename... Remain>
    OperOrganizer(const Head&, const Remain&...) = default;
};

template <typename OpTag>
class OperOrganizer<OpTag, CategoryTags::BatchScalar>
{
private:
    template <typename Head, typename... Remain>
    bool SameDim(const Head&, const Remain&...)
    {
        return true;
    }

    template <typename Head, typename Cur, typename... Remain>
    bool SameDim(const Head& head, const Cur& cur, const Remain&... remain)
    {
        return head.BatchNum() == cur.BatchNum() &&
               SameDim(cur, rem...);
    }

public:
    template <typename Head, typename... Remain>
    OperOrganizer(const Head& head, const Remain&... remain)
        : batch_num(head.BatchNum())
    {
        assert(SameDim(head, remain...));
    }

    size_t BatchNum() const { return batch_num; }

private:
    size_t batch_num;
};

template <typename OpTag>
class OperOrganizer<OpTag, CategoryTags::Matrix>
{
private:
    template <typename Head, typename... Remain>
    bool SameDim(const Head&, const Remain&...)
    {
        return true;
    }

    template <typename Head, typename Cur, typename... Remain>
    bool SameDim(const Head& head, const Cur& cur, const Remain&... remain)
    {
        return head.RowNum() == cur.RowNum &&
               head.ColNum() == cur.ColNum() &&
               SameDim(cur, remain...);
    }

public:
    template <typename Head, typename... Remain>
    OperOrganizer(const Head& head, const Remain&... remain)
        : row_num(head.RowNum())
        , col_num(head.ColNum())
    {
        assert(SameDim(head, remain...));
    }

    size_t RowNum() const { return row_num; }
    size_t ColNum() const { return col_num; }

private:
    size_t row_num;
    size_t col_num;
};

template <typename OpTag>
class OperOrganizer<OpTag, CategoryTags::BatchMatrix>
{
private:
    template <typename Head, typename... Remain>
    bool SameDim(const Head&, const Remain&...)
    {
        return true;
    }

    template <typename Head, typename Cur, typename... Remain>
    bool SameDim(const Head& head, const Cur& cur, const Remain&... remain)
    {
        return head.RowNum() == cur.RowNum() &&
               head.ColNum() == cur.ColNum() &&
               head.BatchNum() == cur.BatchNum() &&
               SameDim(cur, remain...);
    }

public:
    template <typename Head, typename... Remain>
    OperOrganizer(const Head& head, const Remain&... remain)
        : row_num(head.RowNum())
        , col_num(head.ColNum())
        , batch_num(head.BatchNum())
    {
        assert(SameDim(head, remain...));
    }

private:
    size_t row_num;
    size_t col_num;
    size_t batch_num;
};