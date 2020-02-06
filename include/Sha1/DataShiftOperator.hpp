#pragma once
#include <cstddef>
#include "TypeDefine.hpp"

class DataShiftOperator final
{
public:
    explicit DataShiftOperator();
    ~DataShiftOperator();
    DataShiftOperator(const DataShiftOperator&) = delete;
    DataShiftOperator& operator=(const DataShiftOperator&) = delete;

    void setData(const Data32Bit& source);
    bool shift(const size_t num);
    Data32Bit getResult() const;

private:
    Data32Bit data;

public:
    static const size_t shiftTopLimit = 32;
};
