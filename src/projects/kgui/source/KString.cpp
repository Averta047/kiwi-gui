//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KString.h"

std::string WStringToString(const std::wstring& wstr)
{
    std::string ConvertedString;
    int RequiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
    
    if (RequiredSize > 0)
    {
        std::vector<char> buffer(RequiredSize);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], RequiredSize, 0, 0);
        ConvertedString.assign(buffer.begin(), buffer.end() - 1);
    }

    return ConvertedString;
}

std::wstring StringToWString(const std::string& str)
{
    std::wstring ConvertedString;
    int RequiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    
    if (RequiredSize > 0)
    {
        std::vector<wchar_t> buffer(RequiredSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], RequiredSize);
        ConvertedString.assign(buffer.begin(), buffer.end() - 1);
    }

    return ConvertedString;
}
