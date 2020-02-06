#pragma once

#include <vector>
#include "TypeDefine.hpp"

class ShaAlgorithm
{
public:
    explicit ShaAlgorithm();
    virtual ~ShaAlgorithm();
    ShaAlgorithm(const ShaAlgorithm&) = delete;
    ShaAlgorithm& operator=(const ShaAlgorithm&) = delete;

    void processBlock(const std::vector<Data32Bit>& blockData,
        Data32Bit& h0, Data32Bit& h1,Data32Bit& h2, Data32Bit& h3, Data32Bit& h4);

private:
    virtual void method(const std::vector<Data32Bit>& blockData,
        Data32Bit& h0, Data32Bit& h1, Data32Bit& h2, Data32Bit& h3, Data32Bit& h4) = 0;
};

