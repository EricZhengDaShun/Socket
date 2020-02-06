#pragma once

#include <vector>
#include "TypeDefine.hpp"

namespace Utility {

    std::vector<unsigned char> fourByteDataToByteStream(const Data32Bit& source);
    std::vector<unsigned char> fourByteDataToByteStream(const std::vector<Data32Bit>& source);

    Data32Bit byteStreamToFourByteData(const unsigned char first,
        const unsigned char second,
        const unsigned char third,
        const unsigned char fourth);

    std::vector<Data32Bit> byteStreamToFourByteData(const std::vector<unsigned char>& source);
}