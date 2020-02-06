#include "WebSocket/PayloadLength.hpp"
#include <algorithm>

PayloadLength::PayloadLength()
{

}

PayloadLength::~PayloadLength()
{

}

void PayloadLength::setValue(const long long int value)
{
    if (value <= static_cast<const long long int >(noHeaderMaxNum)) {
        setNoHeaderValue(static_cast<char>(value));

    } else if (value <= static_cast<const long long int >(header126MaxNum)) {
        setHeader126Value(static_cast<short>(value));

    } else {
        setHeader127Value(value);
    } 
    return;
}

std::vector<char> PayloadLength::getData() const
{
    return buf;
}

void PayloadLength::reset()
{
    buf.clear();
    return;
}

bool PayloadLength::getSizeByteLength(const std::vector<char>& frameData, size_t& length, size_t& value)
{
    static const size_t lengthHeaderPosition = 1;
    static const size_t headerSize = 1;
    static const char lengthHeaderMask = 0x7F;
    static const size_t bitNumOfByte = 8;
    static const size_t header126MaxNumAppendLength = 2;
    static const size_t header127MaxNumAppendLength = 8;
    static const size_t header126LastPosition = lengthHeaderPosition + header126MaxNumAppendLength;
    static const size_t header127LastPosition = lengthHeaderPosition + header127MaxNumAppendLength;

    if (!(lengthHeaderPosition < frameData.size())) return false;

    const char header = frameData[lengthHeaderPosition] & lengthHeaderMask;
    if (static_cast<size_t>(header) <= noHeaderMaxNum) {
        length = headerSize;
        value = static_cast<size_t>(header);

    } else if (static_cast<size_t>(header) <= header126MaxNum) {
        if (!(header126LastPosition < frameData.size())) return false;
        length = headerSize + header126MaxNumAppendLength;

        value = 0;
        size_t shiftByteCount = 0;
        for (size_t count = lengthHeaderPosition + headerSize; count <= header126LastPosition; ++count) {
            value <<= (shiftByteCount * bitNumOfByte);
            value |= static_cast<unsigned char>(frameData[count]);
            ++shiftByteCount;
        }

    } else {
        if (!(header127LastPosition < frameData.size())) return false;
        length = headerSize + header127MaxNumAppendLength;

        value = 0;
        size_t shiftByteCount = 0;
        for (size_t count = lengthHeaderPosition + headerSize; count <= header127MaxNumAppendLength; ++count) {
            value <<= (shiftByteCount * bitNumOfByte);
            value |= static_cast<unsigned char>(frameData[count]);
            ++shiftByteCount;
        }
    }

    return true;
}

void PayloadLength::setNoHeaderValue(const char value)
{
    buf.clear();
    buf.push_back(value);
    return;
}

void PayloadLength::setHeader126Value(const short value)
{
    static const char header = 126;

    buf.clear();
    buf.push_back(header);

    std::vector<char> byteBuf = PayloadLengthTool::splitValue<short>(value);
    std::reverse(byteBuf.begin(), byteBuf.end());
    buf.insert(buf.end(), byteBuf.cbegin(), byteBuf.cend());
    return;
}

void PayloadLength::setHeader127Value(const long long int value)
{
    static const char header = 127;

    buf.clear();
    buf.push_back(header);

    std::vector<char> byteBuf = PayloadLengthTool::splitValue<long long int>(value);
    std::reverse(byteBuf.begin(), byteBuf.end());
    buf.insert(buf.end(), byteBuf.begin(), byteBuf.end());
    return;
}
