#include "FontFactory.h"
#include "ClientFont.h"
//#include "Font.h"

FontInterfaceUPtr FontFactory::create(TranslateInterface::Language lang, FontInterface::Size size) noexcept {
    return std::unique_ptr<FontInterface>(new ClientFont(lang, size));
    //return std::unique_ptr<FontInterface>(new Font(lang, size));
}
