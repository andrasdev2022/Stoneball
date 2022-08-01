#ifndef __FONTFACTORY_H__
#define __FONTFACTORY_H__

#include "FontInterface.h"
#include "TranslateInterface.h"

class FontFactory
{
public:
    static FontInterfaceUPtr create(TranslateInterface::Language lang, FontInterface::Size size) noexcept;
};

#endif // __FONTFACTORY_H__
