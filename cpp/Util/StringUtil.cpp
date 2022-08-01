#include "StringUtil.h"
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

namespace StringUtil {
    string trim(const string& str) {
        if(str.length() == 0)
            return "";
        size_t first = str.find_first_not_of(" \t\n\r\a");
        if (std::string::npos == first) {
            switch(str[0]) {
                case ' ': case '\t': case '\n': case '\r': case '\a':
                    return "";
                break;
                default:
                break;
            }
            return str;
        }
    
        size_t last = str.find_last_not_of(" \t\n\r\a");
        return str.substr(first, (last - first + 1));
    }

    wstring trim(const wstring& str) {
        if(str.length() == 0)
            return L"";
        size_t first = str.find_first_not_of(L" \t\n\r\a");
        if (std::wstring::npos == first) {
            switch(str[0]) {
                case L' ': case L'\t': case L'\n': case L'\r': case L'\a':
                    return L"";
                break;
                default:
                break;
            }
            return str;
        }

        size_t last = str.find_last_not_of(L" \t\n\r\a");
        return str.substr(first, (last - first + 1));
    }

    void str_replace(string& str, const string str1, const string str2) {
        size_t pos = str.find(str1);
        if(pos == string::npos) return;
        str.replace(pos, str1.length(), str2);
    }

    void wstr_replace(wstring& str, const wstring str1, const wstring str2) {
        size_t pos = str.find(str1);
        if(pos == wstring::npos) return;
        str.replace(pos, str1.length(), str2);
    }

    string group(size_t x) {
        stringstream ss;
        string s = to_string(x);
        size_t p = s.length() % 3;
        for(size_t i = 0; i < s.length(); ++i) {
            if(i % 3 == p) {
                ss << ' ';
            }
            ss << s[i];
        }
        return ss.str();
    }

    string removeWhiteSpace(const string& str) {
        stringstream retv;
        std::copy_if(str.begin(), str.end(), std::ostream_iterator<char>(retv, ""), [](char x) { return x != ' ' && x != '\t' && x != '\n' && x != '\a'; });
        return retv.str();
    }
}
