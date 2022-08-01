#ifndef __MENUINTERFACE_H__
#define __MENUINTERFACE_H__

#include <memory>
#include <map>
#include "ObserverTouchEvent.h"
#include "Defines.h"

class MenuInterface;
using MenuInterfaceUPtr = std::unique_ptr<MenuInterface>;
using MenuInterfaceSPtr = std::shared_ptr<MenuInterface>;

class MultiplayerCreateGameMenu;
class AnimatedKnightsForMenu;
class MultiplayerFindGameMenu;
class MultiplayerOneDeviceGameParameters;
class ApplicationMenu;
class SettingsMenu;

class MenuInterface : public ObserverTouchEvent {
public:
    enum class MenuType {
        MAIN,
        MULTIPLAYER,
        ONLINE,
        SINGLEPLAYER_RUNNIG,
        MULTIPLAYER_ON_ONE_DEVICE_RUNNING,
        MULTIPLAYER_CREATE_GAME,
        MULTIPLAYER_FIND_GAME,
        MULTIPLAYER_HOST_RUNNING,
        MULTIPLAYER_JOINED_RUNNING,
        SETTINGS,
        LEVELS,
        ABOUT,
        MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS,
    };

    virtual ~MenuInterface() = default;
    virtual void update([[maybe_unused]]double now) noexcept {}
    virtual bool draw(AndroidBitmapInfo* info, void* pixels) noexcept = 0;
    virtual void add(MenuInterfaceSPtr subMenu) noexcept = 0;
    virtual MenuInterfaceSPtr& parent() noexcept = 0;
    virtual MenuType menuType() const noexcept = 0;
    virtual MenuInterfaceSPtr findMenu(MenuType t) noexcept = 0;
    virtual std::map<MenuType, MenuInterfaceSPtr>& children() noexcept = 0;
    virtual int drawXOffset() noexcept {return 0;}
    virtual int drawYOffset() noexcept {return 0;}

    // dynamic_cast
    virtual MultiplayerCreateGameMenu* multiplayerCreateGameMenu() noexcept {return nullptr;}
    virtual AnimatedKnightsForMenu* animatedKnightsForMenu() noexcept {return nullptr;}
    virtual MultiplayerFindGameMenu* multiplayerFindGameMenu() noexcept {return nullptr;}
    virtual MultiplayerOneDeviceGameParameters* multiplayerOneDeviceGameParameters() noexcept {return nullptr;}
    virtual ApplicationMenu* applicationMenu() noexcept {return nullptr;}
    virtual SettingsMenu* settingsMenu() noexcept {return nullptr;}
};

#endif // __MENUINTERFACE_H__
