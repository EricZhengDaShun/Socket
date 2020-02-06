#pragma once

namespace Base64 
{
    const char alphabet[65] =
    {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "abcdefghijklmnopqrstuvwxyz" \
        "0123456789+/"
    };

    static const size_t minDataGroupSzie = 3;
    static const size_t minBaseDataGroupSize = 4;
    static const char representComplementNull = '=';

}

