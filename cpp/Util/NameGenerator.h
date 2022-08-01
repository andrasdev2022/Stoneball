#ifndef __NAMEGENERATOR_H__
#define __NAMEGENERATOR_H__

#include <string>
#include <unordered_set>
#include "TranslateInterface.h"

class NameGenerator
{
public:
    NameGenerator();

    std::wstring generate(TranslateInterface::Language lang, bool unique) noexcept;

private:
    std::wstring generate1(TranslateInterface::Language lang, bool unique) noexcept;
    std::wstring generate2(TranslateInterface::Language lang, bool unique) noexcept;
    std::unordered_set<std::wstring> generatedNames_;
};

#endif // __NAMEGENERATOR_H__
