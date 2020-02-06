#include "Sha1/MethodTwo.hpp"
#include <vector>
#include "Sha1/DataShiftOperator.hpp"
#include "Sha1/LogicFunctionExecutor.hpp"
#include "Sha1/Utility.hpp"

MethodTwo::MethodTwo()
    : a(0)
    , b(0)
    , c(0)
    , d(0)
    , e(0)
    , dataShiftOperator(std::make_shared<DataShiftOperator>())
    , logicFunctionExecutor(std::make_shared<LogicFunctionExecutor>())
{

}

MethodTwo::~MethodTwo()
{

}

void MethodTwo::method(const std::vector<Data32Bit>& blockData,
    Data32Bit& h0, Data32Bit& h1, Data32Bit& h2, Data32Bit& h3, Data32Bit& h4)
{
    static const Data32Bit mask = 0x0000000F;
    std::vector<Data32Bit> processData = blockData;

    a = h0;
    b = h1;
    c = h2;
    d = h3;
    e = h4;
    
    Data32Bit tmp = 0;
    for (size_t count = 0; count <= 79; ++count) {
        const Data32Bit sNum = count & mask;
        if (count >= 16) {
            Data32Bit partOne = processData[(sNum + 13) & mask] ^ processData[(sNum + 8) & mask] ^ processData[(sNum + 2) & mask] ^ processData[sNum];
            dataShiftOperator->setData(partOne);
            dataShiftOperator->shift(1);
            processData[sNum] = dataShiftOperator->getResult();
        }
    
        dataShiftOperator->setData(a);
        dataShiftOperator->shift(5);
    
        logicFunctionExecutor->setData(b, c, d);
        logicFunctionExecutor->execute(count);
    
        tmp = dataShiftOperator->getResult() + logicFunctionExecutor->getResult() + e + processData[sNum] + LogicFunctionExecutor::getKey(count);
        e = d;
        d = c;
    
        dataShiftOperator->setData(b);
        dataShiftOperator->shift(30);
        c = dataShiftOperator->getResult();
    
        b = a;
        a = tmp;
    }
    
    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
    return;
}