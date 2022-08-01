#include "TranslatorFromAndroidStrings.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <codecvt>
#include "Application.h"

TranslatorFromAndroidStrings::TranslatorFromAndroidStrings()
{
    std::wstring line;
    std::wregex pattern {LR"x(<string name="(.+_(?:hun|eng|bul))">(.+)<\/string>)x"};
    std::wsmatch matches;
    std::wifstream file("../res/values/strings.xml", std::ios::in);
    std::locale empty;
    file.imbue(std::locale(empty, new std::codecvt_utf8<wchar_t>));

    while(!file.eof()) {
        getline(file, line);
        if(std::regex_search(line, matches, pattern) && matches.size() > 0) {
            translations_[matches[1].str()] = matches[2].str();
        }
    }
}

std::wstring TranslatorFromAndroidStrings::operator()([[maybe_unused]]std::wstring key) noexcept {
    std::map<std::wstring, std::wstring>::const_iterator it = translations_.end();

    switch(Application::language()) {
    case TranslateInterface::Language::ENGLISH: it = translations_.find(key + L"_eng"); break;
    case TranslateInterface::Language::HUNGARIAN: it = translations_.find(key + L"_hun"); break;
    case TranslateInterface::Language::BULGARIAN: it = translations_.find(key + L"_bul"); break;
    case TranslateInterface::Language::LAST_ONE: break;
    }
    if(it == translations_.end()) {
        return L"MISSING_ANDROID_TRANSLATION";
    }
    return it->second;
}
