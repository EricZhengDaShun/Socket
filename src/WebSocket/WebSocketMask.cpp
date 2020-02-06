#include "WebSocket/WebSocketMask.hpp"
#include <random>

WebSocketMask::WebSocketMask()
    : buf(maskSize)
{

}

WebSocketMask::~WebSocketMask()
{

}

void WebSocketMask::setMask(const char first, const char second, const char third, const char fourth)
{
    buf[0] = first;
    buf[1] = second;
    buf[2] = third;
    buf[3] = fourth;
    return;
}

void WebSocketMask::makeRandomMask()
{
    std::random_device randomDevice;
    const unsigned int randomSeedValue = randomDevice();
    std::default_random_engine randomEngine{ randomSeedValue };
    std::uniform_int_distribution<int> uniformIntDis{ 0, 255 };

    for (size_t count = 0; count < buf.size(); ++count) {
        const char randomValue = static_cast<char>(uniformIntDis(randomEngine));
        buf[count] = randomValue;
    }
    return;
}

std::vector<char> WebSocketMask::getData() const
{
    return buf;
}

void WebSocketMask::reset()
{
    for (auto it = buf.begin(); it != buf.end(); ++it) {
        *it = 0;
    }
    return;
}