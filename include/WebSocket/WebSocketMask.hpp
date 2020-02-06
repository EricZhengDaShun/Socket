#pragma once
#include <vector>

class WebSocketMask final
{
public:
    explicit WebSocketMask();
    ~WebSocketMask();
    WebSocketMask(const WebSocketMask&) = delete;
    WebSocketMask& operator=(const WebSocketMask&) = delete;

    void setMask(const char first, const char second, const char third, const char fourth);
    void makeRandomMask();
    std::vector<char> getData() const;

    void reset();

public:
    static const size_t maskSize = 4;

private:
    std::vector<char> buf;
};

