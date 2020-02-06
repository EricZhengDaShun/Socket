#pragma once
#include <vector>
#include "WebSocket/Opcode.hpp"

class FrameParser final
{
public:
    explicit FrameParser();
    ~FrameParser();
    FrameParser(const FrameParser&) = delete;
    FrameParser& operator=(const FrameParser&) = delete;

    void setData(const std::vector<char>& source);
    bool parse();
    void reset();

    bool getFinState() const;
    Opcode::Type getOpcode() const;
    size_t getPayloadLength() const;
    bool getMaskState() const;
    std::vector<char> getMask() const;
    std::vector<char> getPayload() const;

private:
    bool parseFin();
    bool parseOpcode();
    bool parseMaskAndPayloadLength();
    bool parsePayload();

private:
    std::vector<char> buf;
    bool isFin;
    Opcode::Type opcode;
    size_t payloadSize;
    size_t currentParsePosition;
    bool hasMask;
    std::vector<char> maskBuf;
    std::vector<char> payloadBuf;
};