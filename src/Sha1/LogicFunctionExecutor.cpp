#include "Sha1/LogicFunctionExecutor.hpp"

LogicFunctionExecutor::LogicFunctionExecutor()
    : dataB(0)
    , dataC(0)
    , dataD(0)
    , data(0)
{

}

LogicFunctionExecutor::~LogicFunctionExecutor()
{

}

void LogicFunctionExecutor::setData(const Data32Bit& b, const Data32Bit& c, const Data32Bit& d)
{
    dataB = b;
    dataC = c;
    dataD = d;
    return;
}

bool LogicFunctionExecutor::execute(const size_t intervalNum)
{
    bool result = false;
    if (intervalNum <= ConditionLimit::First) {
        result = true;
        firstFunction();

    } else if (intervalNum <= ConditionLimit::Second) {
        result = true;
        secondFunction();

    } else if (intervalNum <= ConditionLimit::Third) {
        result = true;
        thirdFunction();

    } else if (intervalNum <= ConditionLimit::Fourth) {
        result = true;
        fourthFunction();
    }

    return result;
}

Data32Bit LogicFunctionExecutor::getResult() const
{
    return data;
}

Data32Bit LogicFunctionExecutor::getKey(const size_t intervalNum)
{
    Data32Bit result = 0;
    if (intervalNum <= ConditionLimit::First) {
        result = 0x5A827999;

    } else if (intervalNum <= ConditionLimit::Second) {
        result = 0x6ED9EBA1;

    } else if (intervalNum <= ConditionLimit::Third) {
        result = 0x8F1BBCDC;

    } else if (intervalNum <= ConditionLimit::Fourth) {
        result = 0xCA62C1D6;
    }

    return result;
}

void LogicFunctionExecutor::firstFunction()
{
    const Data32Bit partOne = dataB & dataC;

    const Data32Bit notB = ~dataB;
    const Data32Bit partTwo = notB & dataD;

    data = partOne | partTwo;
    return;
}

void LogicFunctionExecutor::secondFunction()
{
    const Data32Bit firstStep = dataB ^ dataC;
    data = firstStep ^ dataD;
    return;
}

void LogicFunctionExecutor::thirdFunction()
{
    const Data32Bit partOne = dataB & dataC;
    const Data32Bit partTwo = dataB & dataD;
    const Data32Bit partThird = dataC & dataD;
    const Data32Bit firstStep = partOne | partTwo;
    data = firstStep | partThird;
    return;
}

void LogicFunctionExecutor::fourthFunction()
{
    const Data32Bit firstStep = dataB ^ dataC;
    data = firstStep ^ dataD;
    return;
}
