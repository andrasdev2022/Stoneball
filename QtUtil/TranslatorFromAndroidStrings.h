#ifndef __TRANSLATORFROMANDROIDSTRINGS_H__
#define __TRANSLATORFROMANDROIDSTRINGS_H__

#include <map>
#include <string>

class TranslatorFromAndroidStrings
{
public:
    TranslatorFromAndroidStrings();

    std::wstring operator()(std::wstring key) noexcept;

private:
    std::map<std::wstring, std::wstring> translations_;
};

#endif // __TRANSLATORFROMANDROIDSTRINGS_H__
