#include "MenuBuilder.h"
#include "MainMenu.h"
#include "LevelsMenu.h"
#include "MultiplayerMenu.h"
#include "SettingsMenu.h"
#include "AboutMenu.h"
#include "MultiplayerOneDeviceGameParameters.h"
#include "MultiplayerCreateGameMenu.h"
#include "MultiplayerFindGameMenu.h"
#include "AnimatedKnightsForMenu.h"
#include "OnlineMenu.h"

MenuInterfaceSPtr MenuBuilder::buildMainMenu() noexcept {
    MenuInterfaceSPtr mainMenu = MenuInterfaceSPtr(new MainMenu());
    MenuInterfaceSPtr levelsMenu = MenuInterfaceSPtr(new LevelsMenu());
    mainMenu->add(levelsMenu);

    MenuInterfaceSPtr multiplayerMenu = MenuInterfaceSPtr(new MultiplayerMenu());
    mainMenu->add(multiplayerMenu);

    MenuInterfaceSPtr oneDeviceParamsMenu = MenuInterfaceSPtr(new MultiplayerOneDeviceGameParameters());
    multiplayerMenu->add(oneDeviceParamsMenu);

    MenuInterfaceSPtr onlineMenu = MenuInterfaceSPtr(new OnlineMenu());
    multiplayerMenu->add(onlineMenu);

    MenuInterfaceSPtr createGameMenu = MenuInterfaceSPtr(new MultiplayerCreateGameMenu());
    onlineMenu->add(createGameMenu);

    MenuInterfaceSPtr findGameMenu = MenuInterfaceSPtr(new MultiplayerFindGameMenu());
    onlineMenu->add(findGameMenu);

    mainMenu->add(MenuInterfaceSPtr(new SettingsMenu()));
    mainMenu->add(MenuInterfaceSPtr(new AboutMenu()));

    // Menuitems which has no drawing
    oneDeviceParamsMenu->add(MenuInterfaceSPtr(new ApplicationMenu(MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING)));
    findGameMenu->add(MenuInterfaceSPtr(new ApplicationMenu(MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING)));
    createGameMenu->add(MenuInterfaceSPtr(new ApplicationMenu(MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING)));
    levelsMenu->add(MenuInterfaceSPtr(new ApplicationMenu(MenuInterface::MenuType::SINGLEPLAYER_RUNNIG)));
    return mainMenu;
}
