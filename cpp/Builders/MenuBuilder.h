#ifndef __MENUBUILDER_H__
#define __MENUBUILDER_H__

#include "MenuInterface.h"

class MenuBuilder {
public:
    static MenuInterfaceSPtr buildMainMenu() noexcept;
};

#endif // __MENUBUILDER_H__
