#pragma once
#include <string>
#include <vector>

class Base64Encoder
{
public:
    explicit Base64Encoder();
    virtual ~Base64Encoder();

    static void encode(const char* const source, const size_t sourceSize, std::string& direct);
    static bool encode(const char* const source, const size_t sourceSize, char* const direct, const size_t directSize);
    static void encode(const std::string& source, std::string& direct);
    static std::string encode(const std::string& source);
    static void encode(const std::vector<char>& source, std::vector<char>& direct);

private:
    static void dataGroupToBaseDataGroup(const char* const source, char* const direct);
    static void dataGroupToBaseDataGroupLast(const char* const source, char* const direct, const size_t complement);

};

