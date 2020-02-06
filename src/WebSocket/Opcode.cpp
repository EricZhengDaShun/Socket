#include "WebSocket/Opcode.hpp"

Opcode::Opcode()
    : type(Opcode::Type::Continuation)
{

}

Opcode::~Opcode()
{

}

Opcode::Opcode(const Opcode& rhv)
    : type(rhv.type)
{

}

Opcode& Opcode::operator=(const Opcode& rhv)
{
    type = rhv.type;
    return *this;
}

void Opcode::setContinuation()
{
    type = Opcode::Type::Continuation;
    return;
}

void Opcode::setText()
{
    type = Opcode::Type::Text;
    return;
}

void Opcode::setBinary()
{
    type = Opcode::Type::Binary;
    return;
}

void Opcode::setConnectionClose()
{
    type = Opcode::Type::ConnectionClose;
    return;
}

void Opcode::setPing()
{
    type = Opcode::Type::Ping;
    return;
}

void Opcode::setPong()
{
    type = Opcode::Type::Pong;
    return;
}

char Opcode::getCode() const
{
    return static_cast<char>(type);
}

void Opcode::reset()
{
    type = Opcode::Type::Continuation;
    return;
}

bool Opcode::isValid(const char source)
{
    bool result = false;
    switch (static_cast<int>(source)) {
        case static_cast<int>(Opcode::Type::Continuation) :
        case static_cast<int>(Opcode::Type::Text) :
        case static_cast<int>(Opcode::Type::Binary) :
        case static_cast<int>(Opcode::Type::ConnectionClose) :
        case static_cast<int>(Opcode::Type::Ping) :
        case static_cast<int>(Opcode::Type::Pong) :
            result = true;
        break;

    default:
        break;
    }

    return result;
}
