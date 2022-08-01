#ifndef __STRINGUTIL_H__
#define __STRINGUTIL_H__

#include <string>

namespace StringUtil {
    std::string trim(const std::string& str);
    std::wstring trim(const std::wstring& str);
    void str_replace(std::string& str, const std::string str1, const std::string str2);
    std::string group(size_t x);
    std::string removeWhiteSpace(const std::string& str);
    void wstr_replace(std::wstring& str, const std::wstring str1, const std::wstring str2);
}

#endif // __STRINGUTIL_H__
