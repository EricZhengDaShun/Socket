#include "Sha1/Utility.hpp"

namespace Utility {

    std::vector<unsigned char> fourByteDataToByteStream(const Data32Bit& source)
    {
        const unsigned char* bitNum = reinterpret_cast<const unsigned char*>(&source);
        std::vector<unsigned char> result;
        result.reserve(4);
        result.push_back(bitNum[3]);
        result.push_back(bitNum[2]);
        result.push_back(bitNum[1]);
        result.push_back(bitNum[0]);
        return result;
    }

    std::vector<unsigned char> fourByteDataToByteStream(const std::vector<Data32Bit>& source)
    {
        std::vector<unsigned char> result;
        result.reserve(source.size() * 4);
        std::vector<unsigned char> tmp;
        for (const Data32Bit& data : source) {
            tmp = fourByteDataToByteStream(data);
            result.insert(result.cend(), tmp.cbegin(), tmp.cend());
        }
        return result;
    }

    Data32Bit byteStreamToFourByteData(const unsigned char first,
        const unsigned char second,
        const unsigned char third,
        const unsigned char fourth)
    {
        Data32Bit result = 0;

        static const size_t bitNumOfOneByte = 8;
        result |= first << (sizeof(unsigned char) * bitNumOfOneByte * 3);
        result |= second << (sizeof(unsigned char) * bitNumOfOneByte * 2);
        result |= third << (sizeof(unsigned char) * bitNumOfOneByte * 1);
        result |= fourth << (sizeof(unsigned char) * bitNumOfOneByte * 0);
        return result;
    }

    std::vector<Data32Bit> byteStreamToFourByteData(const std::vector<unsigned char>& source)
    {
        static const size_t groupByteNum = 4;
        std::vector<Data32Bit> result;
        for (size_t count = 0; count < source.size(); count += groupByteNum) {
            const Data32Bit tmp = byteStreamToFourByteData(source[count  + 0],
                source[count + 1],
                source[count + 2],
                source[count + 3]);

            result.push_back(tmp);
        }
        return result;
    }
}