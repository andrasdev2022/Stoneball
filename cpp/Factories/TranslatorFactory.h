#ifndef __TRANSLATORFACTORY_H__
#define __TRANSLATORFACTORY_H__

#include "TranslateInterface.h"

class TranslatorFactory
{
public:
    static TranslateInterfaceUPtr create(TranslateInterface::Language lang) noexcept;
};

#endif // __TRANSLATORFACTORY_H__
