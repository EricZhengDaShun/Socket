#include "WebSocket/FrameParser.hpp"
#include "WebSocket/PayloadLength.hpp"
#include "WebSocket/WebSocketMask.hpp"

FrameParser::FrameParser()
    : isFin(false)
    , opcode(Opcode::Type::Continuation)
    , payloadSize(0)
    , currentParsePosition(0)
    , hasMask(false)
{

}

FrameParser::~FrameParser()
{

}

void FrameParser::setData(const std::vector<char>& source)
{
    buf.assign(source.cbegin(), source.cend());
    return;
}

bool FrameParser::parse()
{
    if (!parseFin()) return false;
    if (!parseOpcode()) return false;
    if (!parseMaskAndPayloadLength()) return false;
    if (!parsePayload()) return false;
    return true;
}

void FrameParser::reset()
{
    buf.clear();
    isFin = false;
    opcode = Opcode::Type::Continuation;
    payloadSize = 0;
    currentParsePosition = 0;
    hasMask = false;
    maskBuf.clear();
    payloadBuf.clear();
    return;
}

bool FrameParser::getFinState() const
{
    return isFin;
}

Opcode::Type FrameParser::getOpcode() const
{
    return opcode;
}

size_t FrameParser::getPayloadLength() const
{
    return payloadSize;
}

bool FrameParser::getMaskState() const
{
    return hasMask;
}

std::vector<char> FrameParser::getMask() const
{
    return maskBuf;
}

std::vector<char> FrameParser::getPayload() const
{
    return payloadBuf;
}

bool FrameParser::parseFin()
{
    static const size_t bytePosition = 0;
    static const size_t bitPosition = 7;
    static const char mask = 0x01;
    if (buf.empty()) return false;

    const char rawData = buf[bytePosition];
    const char bitData = static_cast<const char>(rawData &
            static_cast<const unsigned char>(mask << bitPosition));
    if (bitData == 0) {
        isFin = false;

    } else {
        isFin = true;
    }

    return true;
}

bool FrameParser::parseOpcode()
{
    static const size_t bytePosition = 0;
    static const char mask = 0x0F;
    if (buf.empty()) return false;

    const char rawData = buf[bytePosition];
    const char data = rawData & mask;
    if (!Opcode::isValid(data)) return false;
    opcode = static_cast<Opcode::Type>(data);
    currentParsePosition += 1;
    return true;
}

bool FrameParser::parseMaskAndPayloadLength()
{
    size_t length = 0;
    if (!PayloadLength::getSizeByteLength(buf, length, payloadSize)) return false;
    currentParsePosition += length;

    static const size_t maskBytePosition = 1;
    static const size_t maskBitPosition = 7;
    static const size_t charMask = 0x01;
    if (!(maskBytePosition < buf.size())) return false;
    const char rawData = buf[maskBytePosition];
    const char bitValue = rawData & static_cast<char>(charMask << maskBitPosition);
    hasMask = (bitValue != 0);
    if (!hasMask) return true;
    const size_t maskNextPosition = currentParsePosition + WebSocketMask::maskSize;
    if (!(maskNextPosition <= buf.size())) return false;
    for (size_t count = 0; count < WebSocketMask::maskSize; ++count) {
        const size_t position = currentParsePosition + count;
        maskBuf.push_back(buf[position]);
    }
    currentParsePosition += WebSocketMask::maskSize;

    return true;
}

bool FrameParser::parsePayload()
{
    if (payloadSize == 0) return true;
    const size_t payloadNextPosition = currentParsePosition + payloadSize;
    if (!(payloadNextPosition <= buf.size())) return false;
    payloadBuf.assign(buf.cbegin() + static_cast<const long int>(currentParsePosition),
                      buf.cbegin() + static_cast<const long int>(payloadNextPosition));
    currentParsePosition += payloadSize;
    return true;
}
