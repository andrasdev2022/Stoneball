#include "FontFactory.h"
#include "font_mock.h"

FontInterfaceUPtr FontFactory::create([[maybe_unused]]TranslateInterface::Language lang, [[maybe_unused]]FontInterface::Size size) noexcept {
    return std::unique_ptr<FontInterface>(new font_mock());
}
