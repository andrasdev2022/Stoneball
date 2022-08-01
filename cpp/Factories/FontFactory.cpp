#include "FontFactory.h"
#include "AndroidFont.h"

FontInterfaceUPtr FontFactory::create(TranslateInterface::Language lang, FontInterface::Size size) noexcept {
    return std::unique_ptr<FontInterface>(new AndroidFont(lang, size));
}

