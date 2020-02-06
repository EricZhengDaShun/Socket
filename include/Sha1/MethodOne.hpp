#pragma once

#include <memory>
#include "ShaAlgorithm.hpp"
#include "TypeDefine.hpp"

class DataShiftOperator;
class LogicFunctionExecutor;

class MethodOne : public ShaAlgorithm
{
public:
    explicit MethodOne();
    virtual ~MethodOne();
    MethodOne(const MethodOne&) = delete;
    MethodOne& operator=(const MethodOne&) = delete;

private:
    void method(const std::vector<Data32Bit>& blockData,
        Data32Bit& h0, Data32Bit& h1, Data32Bit& h2, Data32Bit& h3, Data32Bit& h4) override;

private:
    Data32Bit a;
    Data32Bit b;
    Data32Bit c;
    Data32Bit d;
    Data32Bit e;

    std::shared_ptr<DataShiftOperator> dataShiftOperator;
    std::shared_ptr<LogicFunctionExecutor> logicFunctionExecutor;
};