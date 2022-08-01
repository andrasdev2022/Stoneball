#include "TranslatorFactory.h"
#include "Translator.h"


TranslateInterfaceUPtr TranslatorFactory::create(TranslateInterface::Language lang) noexcept {
    return std::unique_ptr<Translator>(new Translator(lang));
}
