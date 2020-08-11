#pragma once
#include "meta_nn/data/matrices/cpu_matrix.h"
#include <stdexcept>

template <typename Elem>
void DataCopy(const Matrix<Elem, DeviceTags::CPU>& src,
              Matrix<Elem, DeviceTags::CPU>& dst)
{
    const size_t row_num = src.RowNum();
    const size_t col_num = src.ColNum();
    if (row_num != dst.RowNum() || col_num != dst.ColNum())
    {
        throw std::runtime_error("Error in data-copy: Matrix dimension mismatch.");
    }

    const auto mem_src = LowerAccess(src);
    auto mem_dst = LowerAccess(dst);

    const size_t src_pack_num = mem_src.RowLen();
    const size_t dst_pack_num = mem_dst.RowLen();

    const Elem* r1 = mem_src.RawMemory();
    Elem* r = mem_dst.RawMemory();

    if (src_pack_num == col_num && dst_pack_num == col_num)
    {
        memcpy(r, r1, sizeof(Elem) * row_num * col_num);
    }
    else
    {
        for (size_t i = 0; i < row_num; ++i)
        {
            memcpy(r, r1, sizeof(Elem) * col_num);
            r += dst_pack_num;
            r1 += src_pack_num;
        }
    }
}