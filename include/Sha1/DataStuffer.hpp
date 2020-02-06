#pragma once
#include <cstddef>
#include <vector>
#include "TypeDefine.hpp"

class DataStuffer final
{
public:
    ~DataStuffer();
    DataStuffer(const DataStuffer&) = delete;
    DataStuffer& operator=(const DataStuffer&) = delete;

public:
    static void padding(std::vector<unsigned char>& data);

private:
    static void getDataBitNum(const size_t originSize, Data32Bit& highNum, Data32Bit& lowNum);

public:
    static const size_t blockSize = 64;
    static const unsigned char tailData = 0x80;
    static const size_t lengthSize = 8;

private:
    DataStuffer();
};

