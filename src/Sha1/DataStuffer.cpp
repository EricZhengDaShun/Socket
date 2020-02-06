#include "Sha1/DataStuffer.hpp"
#include "Sha1/Utility.hpp"

DataStuffer::DataStuffer()
{

}

DataStuffer::~DataStuffer()
{

}

void DataStuffer::padding(std::vector<unsigned char>& data)
{
    static const size_t tailSize = sizeof(tailData);
    static const size_t alignNeedMinSize = tailSize + lengthSize;
    const size_t originDataSize = data.size();
    const size_t remainder = originDataSize % blockSize;
    size_t complement = blockSize - remainder;
    size_t zeroByteSize = 0;
    if (complement < alignNeedMinSize) {
        complement += blockSize;
    }
    zeroByteSize = complement - alignNeedMinSize;

    Data32Bit highOriginDataBitNum = 0;
    Data32Bit lowOriginDataBitNum = 0;
    getDataBitNum(originDataSize, highOriginDataBitNum, lowOriginDataBitNum);
    const std::vector<unsigned char> highBitNum =  Utility::fourByteDataToByteStream(highOriginDataBitNum);
    const std::vector<unsigned char> lowBitNum = Utility::fourByteDataToByteStream(lowOriginDataBitNum);

    data.reserve(data.size() + complement);
    const unsigned char tailDataTmp = tailData;  // for g++ build ?!
    data.push_back(tailDataTmp);
    data.insert(data.cend(), zeroByteSize, 0x00);
    data.insert(data.cend(), highBitNum.cbegin(), highBitNum.cend());
    data.insert(data.cend(), lowBitNum.cbegin(), lowBitNum.cend());
    return;
}

void DataStuffer::getDataBitNum(const size_t originSize, Data32Bit& highNum, Data32Bit& lowNum)
{
    static const size_t originTopLimit = 0x1FFFFFFF;
    static const size_t bitNumOfOneByte = 8;
    const size_t highByteNum = originSize / originTopLimit;
    highNum = static_cast<Data32Bit>(highByteNum * bitNumOfOneByte);
    const size_t lowByteNum = originSize % originTopLimit;
    lowNum = static_cast<Data32Bit>(lowByteNum * bitNumOfOneByte);
    return;
}
