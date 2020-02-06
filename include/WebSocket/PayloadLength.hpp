#pragma once
#include <vector>

namespace PayloadLengthTool {
    template<typename T>
    std::vector<char> splitValue(const T& value)
    {
        static const unsigned char byteMask = 0xFF;
        static const size_t bitNumOfByte = 8;

        std::vector<char> byteBuf;
        const size_t totalByteNum = sizeof(value);
        for (size_t count = 0; count < totalByteNum; ++count) {
            const size_t shifftBit = bitNumOfByte * count;
            const T shiftTmp = value >> shifftBit;
            const char part = static_cast<char>(shiftTmp & byteMask);
            byteBuf.push_back(part);
        }
        return byteBuf;
    }
}

class PayloadLength final
{
public:
    explicit PayloadLength();
    ~PayloadLength();
    PayloadLength(const PayloadLength&) = delete;
    PayloadLength& operator=(const PayloadLength&) = delete;

    void setValue(const long long int value);
    std::vector<char> getData() const;

    void reset();

public:
    static bool getSizeByteLength(const std::vector<char>& frameData, size_t& length, size_t& value);

private:
    void setNoHeaderValue(const char value);
    void setHeader126Value(const short value);
    void setHeader127Value(const long long int value);

private:
    std::vector<char> buf;

private:
    static const size_t noHeaderMaxNum = 125;
    static const size_t header126MaxNum = 0xFF;
};

