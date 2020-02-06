#include "Sha1/MethodOne.hpp"
#include <vector>
#include "Sha1/DataShiftOperator.hpp"
#include "Sha1/LogicFunctionExecutor.hpp"

MethodOne::MethodOne()
    : a(0)
    , b(0)
    , c(0)
    , d(0)
    , e(0)
    , dataShiftOperator(std::make_shared<DataShiftOperator>())
    , logicFunctionExecutor(std::make_shared<LogicFunctionExecutor>())
{

}

MethodOne::~MethodOne()
{

}

void MethodOne::method(const std::vector<Data32Bit>& blockData,
    Data32Bit& h0, Data32Bit& h1, Data32Bit& h2, Data32Bit& h3, Data32Bit& h4)
{
    std::vector<Data32Bit> data = blockData;
    for (size_t count = 16; count <= 79; ++count) {
        dataShiftOperator->setData(data[count - 3] ^ data[count - 8] ^ data[count - 14] ^ data[count - 16]);
        dataShiftOperator->shift(1);
        data.push_back(dataShiftOperator->getResult());
    }

    a = h0;
    b = h1;
    c = h2;
    d = h3;
    e = h4;

    Data32Bit tmp = 0;
    for (size_t count = 0; count <= 79; ++count) {

        dataShiftOperator->setData(a);
        dataShiftOperator->shift(5);

        logicFunctionExecutor->setData(b, c, d);
        logicFunctionExecutor->execute(count);

        tmp = dataShiftOperator->getResult() + logicFunctionExecutor->getResult() + e + data[count] + LogicFunctionExecutor::getKey(count);
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