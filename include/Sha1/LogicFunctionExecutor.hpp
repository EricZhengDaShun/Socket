#pragma once
#include <cstddef>
#include "TypeDefine.hpp"

class LogicFunctionExecutor final
{
public:
    explicit LogicFunctionExecutor();
    ~LogicFunctionExecutor();
    LogicFunctionExecutor(const LogicFunctionExecutor&) = delete;
    LogicFunctionExecutor& operator=(const LogicFunctionExecutor&) = delete;

    void setData(const Data32Bit& b, const Data32Bit& c, const Data32Bit& d);
    bool execute(const size_t intervalNum);
    Data32Bit getResult() const;

public:
    static Data32Bit getKey(const size_t intervalNum);

private:
    enum ConditionLimit : size_t
    {
        First = 19,
        Second = 39,
        Third = 59,
        Fourth = 79,
    };

private:
    void firstFunction();
    void secondFunction();
    void thirdFunction();
    void fourthFunction();

private:
    Data32Bit dataB;
    Data32Bit dataC;
    Data32Bit dataD;
    Data32Bit data;
};

