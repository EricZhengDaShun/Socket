#pragma once
#include <string>
#include <vector>

class Base64Decoder
{
public:
    enum class ErrorCode
    {
        success = 0,
        sourceNotAlign,
        decodeError,
        insufficientBuffter
    };

private:
    struct DecodeInformation
    {
        bool isValid;
        size_t remainder;
        size_t complement;
        size_t baseDataGroupNum;
        size_t outputSize;
    };

public:
    explicit Base64Decoder();
    virtual ~Base64Decoder();

    static size_t getDecodeNeedSize(const std::string& source, ErrorCode* const errorCode = nullptr);
    static Base64Decoder::ErrorCode decode(const std::string& source, char* const direct, const size_t directSize);
    static Base64Decoder::ErrorCode decode(const std::string& source, std::string& direct);
    static Base64Decoder::ErrorCode decode(const std::string& source, std::vector<char>& direct);
    static Base64Decoder::ErrorCode decode(const std::vector<char>& source, std::vector<char>& direct);

private:
    static void calculationDecodeInformation(const std::string& source, DecodeInformation& decodeInfo);
    static bool baseDataGroupToDataGroup(const char* const source, char* const direct);
    static bool baseDataGroupToDataGroupLast(const char* const source, char* const direct, const size_t complement);
    static bool base64Lookup(const char source, char& result);

};

