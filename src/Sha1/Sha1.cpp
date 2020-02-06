#include "Sha1/Sha1.hpp"
#include "Sha1/DataStuffer.hpp"
#include "Sha1/Utility.hpp"
#include "Sha1/ShaAlgorithm.hpp"
#include "Sha1/MethodOne.hpp"
#include "Sha1/MethodTwo.hpp"

Sha1::Sha1()
    : h0(0)
    , h1(0)
    , h2(0)
    , h3(0)
    , h4(0)
    , methodOne(std::make_shared<MethodOne>())
    , methodTwo(std::make_shared<MethodTwo>())
{

}

Sha1::~Sha1()
{

}

void Sha1::setData(const std::vector<unsigned char>& source)
{
    dataBuf.clear();
    std::vector<unsigned char> tmp = source;
    DataStuffer::padding(tmp);
    dataBuf = Utility::byteStreamToFourByteData(tmp);
    return;
}

std::vector<unsigned char> Sha1::getResultWithMethodOne()
{
    return getResult(methodOne);
}

std::vector<unsigned char> Sha1::getResultWithMethodTwo()
{
    return getResult(methodTwo);
}

void Sha1::resetResult()
{
    h0 = 0x67452301;
    h1 = 0xEFCDAB89;
    h2 = 0x98BADCFE;
    h3 = 0x10325476;
    h4 = 0xC3D2E1F0;
    return;
}

std::vector<unsigned char> Sha1::getResult(std::shared_ptr<ShaAlgorithm> method)
{
    resetResult();

    static const size_t blockBit = 512;
    static const size_t bitNumOfOneByte = 8;
    static const size_t blockByte = blockBit / bitNumOfOneByte;
    static const size_t blockData32BitNum = blockByte / sizeof(Data32Bit);

    const size_t blockNum = dataBuf.size() / blockData32BitNum;
    std::vector<Data32Bit> processData;
    processData.reserve(blockData32BitNum);
    for (size_t blockCount = 0; blockCount < blockNum; ++blockCount) {
        processData.clear();
        const size_t processDataBeginPos = blockCount * blockData32BitNum;
        auto it = dataBuf.cbegin() + processDataBeginPos;
        processData.insert(processData.cend(), it, it + blockData32BitNum);
        method->processBlock(processData, h0, h1, h2, h3, h4);
    }

    const std::vector<Data32Bit> tmp{ h0, h1, h2, h3, h4 };
    std::vector<unsigned char> result = Utility::fourByteDataToByteStream(tmp);
    return result;
}