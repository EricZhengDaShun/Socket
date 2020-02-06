#pragma once
#include "Base64Encoder.hpp"
#include "Base64Decoder.hpp"

class Base64Processor : public Base64Encoder, public Base64Decoder
{
public:
    explicit Base64Processor();
    virtual ~Base64Processor();

};

