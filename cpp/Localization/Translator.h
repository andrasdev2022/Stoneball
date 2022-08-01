#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__

#include <unordered_map>
#include <array>
#include "TranslateInterface.h"

class Translator : public TranslateInterface
{
    Translator(Language lang) noexcept;
    friend class TranslatorFactory;
public:
    std::wstring operator()(TranslateInterface::Key key) const override;
    std::wstring operator()(Language lng) const override {return operator()(lang2Key(lng));}
    std::wstring str(TranslateInterface::Key key) const noexcept override;
    Language language() const noexcept override {return lang_;}

private:
    void initDictionary() noexcept ;

    Language lang_;
    std::array<std::unordered_map<Key, std::wstring>, static_cast<size_t>(Language::LAST_ONE)> translations;
};

#endif // __TRANSLATOR_H__
