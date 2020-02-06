#include "Base64/Base64Decoder.hpp"
#include "Base64/Base64Define.hpp"

Base64Decoder::Base64Decoder()
{

}

Base64Decoder::~Base64Decoder()
{

}

size_t Base64Decoder::getDecodeNeedSize(const std::string& source, ErrorCode* const errorCode)
{
    DecodeInformation decodeInfo;
    calculationDecodeInformation(source, decodeInfo);
    if ((decodeInfo.isValid == false) && (errorCode != nullptr)) *errorCode = ErrorCode::sourceNotAlign;

    return decodeInfo.outputSize;
}

Base64Decoder::ErrorCode Base64Decoder::decode(const std::string& source, char* const direct, const size_t directSize)
{
    DecodeInformation decodeInfo;
    calculationDecodeInformation(source, decodeInfo);
    if (decodeInfo.isValid == false) return ErrorCode::sourceNotAlign;
    if (directSize < decodeInfo.outputSize) return ErrorCode::insufficientBuffter;

    bool result = false;
    const size_t baseDataGroupNumExceptLast = decodeInfo.baseDataGroupNum - 1;
    size_t dataPosition     = 0;
    size_t baseDataPosition = 0;
    for (size_t count = 0; count < baseDataGroupNumExceptLast; ++count) {
        dataPosition     = Base64::minDataGroupSzie     * count;
        baseDataPosition = Base64::minBaseDataGroupSize * count;
        result = baseDataGroupToDataGroup(&source[baseDataPosition], &direct[dataPosition]);
        if (result == false) return ErrorCode::decodeError;
    }

    const size_t lastDataPosition     = Base64::minDataGroupSzie     * baseDataGroupNumExceptLast;
    const size_t lastBaseDataPosition = Base64::minBaseDataGroupSize * baseDataGroupNumExceptLast;
    result = baseDataGroupToDataGroupLast(&source[lastBaseDataPosition], &direct[lastDataPosition], decodeInfo.complement);
    if (result == false) return ErrorCode::decodeError;

    return ErrorCode::success;
}

Base64Decoder::ErrorCode Base64Decoder::decode(const std::string& source, std::string& direct)
{
    const size_t resultSize = getDecodeNeedSize(source);
    direct.resize(resultSize);
    return decode(source, &direct[0], direct.size());
}

Base64Decoder::ErrorCode Base64Decoder::decode(const std::string& source, std::vector<char>& direct)
{
    const size_t resultSize = getDecodeNeedSize(source);
    direct.resize(resultSize);
    return decode(source, direct.data(), direct.size());
}

Base64Decoder::ErrorCode Base64Decoder::decode(const std::vector<char>& source, std::vector<char>& direct)
{
    const std::string data(source.begin(), source.end());
    const size_t resultSize = getDecodeNeedSize(data);
    direct.resize(resultSize);
    return decode(data, direct.data(), direct.size());
}

void Base64Decoder::calculationDecodeInformation(const std::string& source, DecodeInformation& decodeInfo)
{
    decodeInfo.remainder = source.size() % Base64::minBaseDataGroupSize;
    if (decodeInfo.remainder != 0 || (source.size() < Base64::minBaseDataGroupSize)) {
        decodeInfo.isValid          = false;
        decodeInfo.complement       = 0;
        decodeInfo.baseDataGroupNum = 0;
        decodeInfo.outputSize       = 0;
        return;
    }
    decodeInfo.isValid = true;

    decodeInfo.complement = 0;
    const char last       = source.rbegin()[0];
    const char secondLast = source.rbegin()[1];
    if (last       == Base64::representComplementNull) ++decodeInfo.complement;
    if (secondLast == Base64::representComplementNull) ++decodeInfo.complement;

    decodeInfo.baseDataGroupNum = source.size() / Base64::minBaseDataGroupSize;
    decodeInfo.outputSize = (decodeInfo.baseDataGroupNum * Base64::minDataGroupSzie) - decodeInfo.complement;

    return;
}

bool Base64Decoder::baseDataGroupToDataGroup(const char* const source, char* const direct)
{
    char firstNum  = '\0';
    char secondNum = '\0';
    char thirdNum  = '\0';
    char fourthNum = '\0';

    if (!base64Lookup(source[0], firstNum)) return false;
    if (!base64Lookup(source[1], secondNum)) return false;
    if (!base64Lookup(source[2], thirdNum)) return false;
    if (!base64Lookup(source[3], fourthNum)) return false;

    direct[0] = (firstNum << 2) + ((secondNum & 0b00110000) >> 4);
    direct[1] = ((secondNum & 0b00001111) << 4) + ((thirdNum & 0b00111100) >> 2);
    direct[2] = ((thirdNum & 0b00000011) << 6) + fourthNum;

    return true;
}

bool Base64Decoder::baseDataGroupToDataGroupLast(const char* const source, char* const direct, const size_t complement)
{
    if (complement == 0) return baseDataGroupToDataGroup(source, direct);

    char firstNum  = '\0';
    char secondNum = '\0';
    char thirdNum  = '\0';

    if (!base64Lookup(source[0], firstNum)) return false;
    if (!base64Lookup(source[1], secondNum)) return false;

    if (complement == 1) {
        if (!base64Lookup(source[2], thirdNum)) return false;
    }

    direct[0] = (firstNum << 2) + ((secondNum & 0b00110000) >> 4);
    if (complement == 1) direct[1] = ((secondNum & 0b00001111) << 4) + ((thirdNum & 0b00111100) >> 2);

    return true;
}

bool Base64Decoder::base64Lookup(const char source, char& result)
{
    result = source;
    if (source >= 'A' && source <= 'Z') result = source - 'A';
    if (source >= 'a' && source <= 'z') result = source - 71;
    if (source >= '0' && source <= '9') result = source + 4;
    if (source == '+') result = 62;
    if (source == '/') result = 63;

    if (result != source) return true;

    return false;
}