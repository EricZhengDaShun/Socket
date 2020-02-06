#include "Base64/Base64Encoder.hpp"
#include "Base64/Base64Define.hpp"

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif 

Base64Encoder::Base64Encoder()
{

}

Base64Encoder::~Base64Encoder()
{

}

void Base64Encoder::encode(const char* const source, const size_t sourceSize, std::string& direct)
{
    const size_t remainder = sourceSize % Base64::minDataGroupSzie;
    size_t complement = 0;
    if (remainder != 0) complement = Base64::minDataGroupSzie - remainder;
    const size_t dataGroupNum = (sourceSize + complement) / Base64::minDataGroupSzie;
    const size_t outputSize = dataGroupNum * Base64::minBaseDataGroupSize;
    direct.resize(outputSize);

    const size_t dataGroupNumExceptLast = dataGroupNum - 1;
    size_t dataPosition     = 0;
    size_t baseDataPosition = 0;
    for (size_t count = 0; count < dataGroupNumExceptLast; ++count) {
        dataPosition     = Base64::minDataGroupSzie     * count;
        baseDataPosition = Base64::minBaseDataGroupSize * count;
        dataGroupToBaseDataGroup(&source[dataPosition], &direct[baseDataPosition]);
    }

    const size_t lastDataPosition     = Base64::minDataGroupSzie     * dataGroupNumExceptLast;
    const size_t lastBaseDataPosition = Base64::minBaseDataGroupSize * dataGroupNumExceptLast;
    dataGroupToBaseDataGroupLast(&source[lastDataPosition], &direct[lastBaseDataPosition], complement);

    return;
}

bool Base64Encoder::encode(const char* const source, const size_t sourceSize, char* const direct, const size_t directSize)
{
    std::string result;
    encode(source, sourceSize, result);
    if (directSize < result.size()) return false;

    result.copy(direct, result.size());
    return true;
}

void Base64Encoder::encode(const std::string& source, std::string& direct)
{
    encode(source.data(), source.size(), direct);
    return;
}

std::string Base64Encoder::encode(const std::string& source)
{
    std::string result;
    encode(source.data(), source.size(), result);
    return result;
}

void Base64Encoder::encode(const std::vector<char>& source, std::vector<char>& direct)
{
    std::string result;
    encode(source.data(), source.size(), result);
    direct.assign(result.begin(), result.end());
    return;
}

void Base64Encoder::dataGroupToBaseDataGroup(const char* const source, char* const direct)
{
    const int firstNum = (source[0] & 0b11111100) >> 2;
    const int secondNum = ((source[0] & 0b00000011) << 4) + ((source[1] & 0b11110000) >> 4);
    const int thirdNum = ((source[1] & 0b00001111) << 2) + ((source[2] & 0b11000000) >> 6);
    const int fourthNum = source[2] & 0b00111111;

    direct[0] = Base64::alphabet[firstNum];
    direct[1] = Base64::alphabet[secondNum];
    direct[2] = Base64::alphabet[thirdNum];
    direct[3] = Base64::alphabet[fourthNum];
    return;
}

void Base64Encoder::dataGroupToBaseDataGroupLast(const char* const source, char* const direct, const size_t complement)
{
    if (complement == 0) {
        dataGroupToBaseDataGroup(source, direct);

    }
    else if (complement == 1) {
        const int firstNum = (source[0] & 0b11111100) >> 2;
        const int secondNum = ((source[0] & 0b00000011) << 4) + ((source[1] & 0b11110000) >> 4);
        const int thirdNum = ((source[1] & 0b00001111) << 2);
        direct[0] = Base64::alphabet[firstNum];
        direct[1] = Base64::alphabet[secondNum];
        direct[2] = Base64::alphabet[thirdNum];
        direct[3] = Base64::representComplementNull;

    }
    else if (complement == 2) {
        const int firstNum = (source[0] & 0b11111100) >> 2;
        const int secondNum = ((source[0] & 0b00000011) << 4);
        direct[0] = Base64::alphabet[firstNum];
        direct[1] = Base64::alphabet[secondNum];
        direct[2] = Base64::representComplementNull;
        direct[3] = Base64::representComplementNull;

    }

    return;
}