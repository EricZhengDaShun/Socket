#pragma once
#include <vector>
#include <string>
#include <memory>
#include "WebSocket/Opcode.hpp"
#include "WebSocket/PayloadLength.hpp"
#include "WebSocket/WebSocketMask.hpp"

class FrameData final
{
public:
    explicit FrameData();
    ~FrameData();
    FrameData(const FrameData&) = delete;
    FrameData& operator=(const FrameData&) = delete;

    void reset();

    void setFin(const bool fin);
    void setOpcode(const Opcode& code);
    void setMask(const char first, const char second, const char third, const char fourth);
    void setRandomMask();
    void setPayloadData(const std::vector<char>& data);

    const std::vector<char>& getData() const;

private:
    char mixFinAndOpcode() const;
    const std::vector<char> getMixMaskAndPayloadLength() const;
    const std::vector<char> getMaskPayload() const;

private:
    bool isFin;
    bool rsv1;
    bool rsv2;
    bool rsv3;
    Opcode opcode;
    bool maskState;
    PayloadLength payloadLength;
    WebSocketMask webSocketMask;
    std::vector<char> payloadData;

    mutable std::vector<char> directBuf;
};

namespace FramTool {
    namespace client {
        inline void setContinuation(std::unique_ptr<FrameData>& frameData, const std::vector<char>& source)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setContinuation();
            frameData->setOpcode(opcode);
            frameData->setRandomMask();
            frameData->setPayloadData(source);
            return;
        }

        inline void setText(std::unique_ptr<FrameData>& frameData, const std::string& text)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setText();
            frameData->setOpcode(opcode);
            frameData->setRandomMask();

            std::vector<char> tmp(text.cbegin(), text.cend());
            frameData->setPayloadData(tmp);
            return;
        }

        inline void setBinary(std::unique_ptr<FrameData>& frameData, const std::vector<char>& source)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setBinary();
            frameData->setOpcode(opcode);
            frameData->setRandomMask();
            frameData->setPayloadData(source);
            return;
        }

        inline void setClose(std::unique_ptr<FrameData>& frameData)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setConnectionClose();
            frameData->setOpcode(opcode);
            std::vector<char> nullBuf;
            frameData->setPayloadData(nullBuf);
            frameData->setRandomMask();
            return;
        }

        inline void setPing(std::unique_ptr<FrameData>& frameData)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setPing();
            frameData->setOpcode(opcode);
            std::vector<char> nullBuf;
            frameData->setPayloadData(nullBuf);
            frameData->setRandomMask();
            return;
        }

        inline void setPong(std::unique_ptr<FrameData>& frameData)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setPong();
            frameData->setOpcode(opcode);
            std::vector<char> nullBuf;
            frameData->setPayloadData(nullBuf);
            frameData->setRandomMask();
            return;
        }
    }
    
    namespace server {
        inline void setContinuation(std::unique_ptr<FrameData>& frameData, const std::vector<char>& source)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setContinuation();
            frameData->setOpcode(opcode);
            frameData->setPayloadData(source);
            return;
        }

        inline void setText(std::unique_ptr<FrameData>& frameData, const std::string& text)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setText();
            frameData->setOpcode(opcode);

            std::vector<char> tmp(text.cbegin(), text.cend());
            frameData->setPayloadData(tmp);
            return;
        }

        inline void setBinary(std::unique_ptr<FrameData>& frameData, const std::vector<char>& source)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setBinary();
            frameData->setOpcode(opcode);
            frameData->setPayloadData(source);
            return;
        }

        inline void setClose(std::unique_ptr<FrameData>& frameData)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setConnectionClose();
            frameData->setOpcode(opcode);
            std::vector<char> nullBuf;
            frameData->setPayloadData(nullBuf);
            return;
        }

        inline void setPing(std::unique_ptr<FrameData>& frameData)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setPing();
            frameData->setOpcode(opcode);
            std::vector<char> nullBuf;
            frameData->setPayloadData(nullBuf);
            return;
        }

        inline void setPong(std::unique_ptr<FrameData>& frameData)
        {
            if (frameData.get() == nullptr) return;

            frameData->reset();
            frameData->setFin(true);
            Opcode opcode;
            opcode.setPong();
            frameData->setOpcode(opcode);
            std::vector<char> nullBuf;
            frameData->setPayloadData(nullBuf);
            return;
        }
    }
}
