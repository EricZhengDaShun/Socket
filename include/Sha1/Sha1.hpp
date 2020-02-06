#pragma once

#include <vector>
#include <memory>
#include "TypeDefine.hpp"

class ShaAlgorithm;

class Sha1 final
{
public:
    explicit Sha1();
    ~Sha1();
    Sha1(const Sha1&) = delete;
    Sha1& operator=(const Sha1&) = delete;

    void setData(const std::vector<unsigned char>& source);
    std::vector<unsigned char> getResultWithMethodOne();
    std::vector<unsigned char> getResultWithMethodTwo();

private:
    void resetResult();
    std::vector<unsigned char> getResult(std::shared_ptr<ShaAlgorithm> method);

private:
    Data32Bit h0;
    Data32Bit h1;
    Data32Bit h2;
    Data32Bit h3;
    Data32Bit h4;

    std::vector<Data32Bit> dataBuf;

    std::shared_ptr<ShaAlgorithm> methodOne;
    std::shared_ptr<ShaAlgorithm> methodTwo;
};

