#include "WebSocket/FrameData.hpp"

namespace {
    bool setCharBitValue(char& reference, const size_t bitNum, const bool value)
    {
        static const size_t bitNumOfByte = 8;
        static const char mask = 0x01;
        if (bitNum >= bitNumOfByte) return false;

        const char bitPosition = static_cast<char>(mask << bitNum);
        const char resetMask = ~bitPosition;
        reference &= resetMask;
        if (value) {
            reference |= bitPosition;
        }
        return true;
    }
}

FrameData::FrameData()
    : isFin(true)
    , rsv1(false)
    , rsv2(false)
    , rsv3(false)
    , maskState(false)
{

}

FrameData::~FrameData()
{

}

void FrameData::reset()
{
    isFin = true;
    rsv1 = false;
    rsv2 = false;
    rsv3 = false;
    opcode.reset();
    maskState = false;
    payloadLength.reset();
    webSocketMask.reset();
    payloadData.clear();
    directBuf.clear();
    return;
}

void FrameData::setFin(const bool fin)
{
    isFin = fin;
    return;
}

void FrameData::setOpcode(const Opcode& code)
{
    opcode = code;
    return;
}

void FrameData::setMask(const char first, const char second, const char third, const char fourth)
{
    maskState = true;
    webSocketMask.setMask(first, second, third, fourth);
    return;
}

void FrameData::setRandomMask()
{
    maskState = true;
    webSocketMask.makeRandomMask();
    return;
}

void FrameData::setPayloadData(const std::vector<char>& data)
{
    payloadData.clear();
    std::copy(data.cbegin(), data.cend(), std::back_inserter(payloadData));
    payloadLength.setValue(payloadData.size());
    return;
}

const std::vector<char>& FrameData::getData() const
{
    directBuf.clear();
    
    const char finAndOpcode = mixFinAndOpcode();
    directBuf.push_back(finAndOpcode);

    const std::vector<char> maskAndPayloadLength = getMixMaskAndPayloadLength();
    directBuf.insert(directBuf.end(), maskAndPayloadLength.cbegin(), maskAndPayloadLength.cend());

    const std::vector<char> maskPayload = getMaskPayload();
    directBuf.insert(directBuf.end(), maskPayload.cbegin(), maskPayload.cend());
    return directBuf;
}

char FrameData::mixFinAndOpcode() const
{
    static const size_t finBitNum  = 7;
    static const size_t rsv1BitNum = 6;
    static const size_t rsv2BitNum = 5;
    static const size_t rsv3BitNum = 4;
    static const char opcodeMask = 0x0F;

    char result = 0;

    setCharBitValue(result, finBitNum, isFin);
    setCharBitValue(result, rsv1BitNum, rsv1);
    setCharBitValue(result, rsv2BitNum, rsv2);
    setCharBitValue(result, rsv3BitNum, rsv3);

    char opcodeByte = opcode.getCode();
    opcodeByte &= opcodeMask;
    result |= opcodeByte;
    return result;
}

const std::vector<char> FrameData::getMixMaskAndPayloadLength() const
{
    static const size_t maskBitNum = 7;
    std::vector<char> result = payloadLength.getData();
    if (result.empty()) return result;

    char& header = result[0];
    setCharBitValue(header, maskBitNum, maskState);
    if (maskState) {
        std::vector<char> maskData = webSocketMask.getData();
        result.insert(result.end(), maskData.cbegin(), maskData.cend());
    }

    return result;
}

const std::vector<char> FrameData::getMaskPayload() const
{
    std::vector<char> result{ payloadData };
    const std::vector<char> maskData = webSocketMask.getData();
    for (size_t count = 0; count < result.size(); ++count) {
        const size_t maskNum = count % maskData.size();
        const char mask = maskData[maskNum];
        result[count] ^= mask;
    }
    return result;
}
