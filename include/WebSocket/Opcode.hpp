#pragma once

class Opcode final
{
public:
    enum class Type : char
    {
        Continuation = 0x00,
        Text = 0x01,
        Binary = 0x02,
        ConnectionClose = 0x08,
        Ping = 0x09,
        Pong = 0x0A
    };

public:
    explicit Opcode();
    ~Opcode();
    Opcode(const Opcode& rhv);
    Opcode& operator=(const Opcode& rhv);

    void setContinuation();
    void setText();
    void setBinary();
    void setConnectionClose();
    void setPing();
    void setPong();

    char getCode() const;

    void reset();

public:
    static bool isValid(const char source);

private:
    Type type;
};