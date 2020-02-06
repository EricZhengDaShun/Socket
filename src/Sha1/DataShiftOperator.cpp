#include "Sha1/DataShiftOperator.hpp"

DataShiftOperator::DataShiftOperator()
    : data(0)
{

}

DataShiftOperator::~DataShiftOperator()
{

}

void DataShiftOperator::setData(const Data32Bit& source)
{
    data = source;
    return;
}

bool DataShiftOperator::shift(const size_t num)
{
    if (num >= shiftTopLimit) return false;

    const Data32Bit firstItem = data << num;
    const Data32Bit secondItem = data >> (shiftTopLimit - num);
    data = firstItem | secondItem;
    return true;
}

Data32Bit DataShiftOperator::getResult() const
{
    return data;
}