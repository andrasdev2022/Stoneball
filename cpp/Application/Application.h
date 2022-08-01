#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "TranslateInterface.h"
#include "FontInterface.h"
#include "LevelFactory.h"
#include <map>
#include <vector>
#include "GameObjectInterface.h"
#include "MenuInterface.h"
#include "GameInterface.h"

class World;
using WorldUPtr = std::unique_ptr<World>;

class StaticObjects;
class Button;
class GameMap;
class Screen;
class TouchEventHandler;
class Collision;
class MenuTouchEvent;
class Environment;
class UDPServerInterface;
class Messagebox1;
class Listbox1;
class BlueTriangle;
using BlueTriangleUPtr = std::unique_ptr<BlueTriangle>;

namespace Application {
    enum class PlayerMode {
        SINGLEPLAYER,
        MULTIPLAYER,
        UNDEFINED
    };

    // For multiplayer world
    enum class Avatar {
        RED,
        DARK,
        BLONDE_FEMALE,
        GREEN_FEMALE,
        UNDEFINED
    };

    enum class PlayMode {
        TURNBASED,  // multiplayer and play on one device
        REALTIME    // levels
    };

    void updateWorld(double now) noexcept;
    void drawWorld(AndroidBitmapInfo* info, void* pixels) noexcept;
    void wakeUp() noexcept;
    void restartLevel() noexcept;
    void goUpInTheMenu() noexcept;

    [[nodiscard]] Screen* screen() noexcept;
    [[nodiscard]] TouchEventHandler* touchEventHandler() noexcept;
    //[[nodiscard]] std::map<double, WorldUPtr>& worldList() noexcept;
    [[nodiscard]] GameInterfaceUPtr& game() noexcept;
    [[nodiscard]] double now() noexcept;
    [[nodiscard]] PlayerMode playerMode() noexcept;
    void setPlayerMode(PlayerMode) noexcept;
    [[nodiscard]] Avatar avatar() noexcept;
    void setAvatar(Avatar a) noexcept;
    [[nodiscard]] MenuInterface::MenuType activeMenu() noexcept;
    [[nodiscard]] MenuInterfaceSPtr activeMenuItem() noexcept;
    void setActiveMenu(MenuInterface::MenuType t) noexcept;
    [[nodiscard]] GameObjectInterface::Type Avatar2GOT(Avatar avatar) noexcept;
    [[nodiscard]] Avatar GOT2Avatar(GameObjectInterface::Type got) noexcept;
    [[nodiscard]] LevelFactory::GameLevel gameLevel() noexcept;
    [[nodiscard]] UDPServerInterface* udpServer() noexcept;
    [[nodiscard]] MenuTouchEvent* menuTouchEvent() noexcept;
    [[nodiscard]] Environment* environment() noexcept;
    [[nodiscard]] Tileset* tileset() noexcept;
    void stop() noexcept;
    [[nodiscard]] WorldUPtr& latestWorld() noexcept;
    [[nodiscard]] TranslateInterface::Language language() noexcept;
    void setLanguage(TranslateInterface::Language ) noexcept;
    [[nodiscard]] const TranslateInterfaceUPtr& translator() noexcept;
    [[nodiscard]] const FontInterfaceUPtr& font() noexcept;
    [[nodiscard]] const FontInterfaceUPtr& font8px() noexcept;
    [[nodiscard]] const FontInterfaceUPtr& font12px() noexcept;
    [[nodiscard]] const FontInterfaceUPtr& font14px() noexcept;
    [[nodiscard]] const FontInterfaceUPtr& font24px() noexcept;
    [[nodiscard]] const FontInterfaceUPtr& font30px() noexcept;

    void freezUpdate() noexcept;
    [[nodiscard]] PlayMode playMode() noexcept;
    void setPlayMode(PlayMode p) noexcept;
    void nextTurn() noexcept;
    void setGameLevel(LevelFactory::GameLevel gameLevel) noexcept;

    // These gameobjects are displayed before the world is created
    void showMessageBox(Messagebox1* messagebox) noexcept;
    void showWidget(BlueTriangle* widget) noexcept;
    void destroyMessageBoxes() noexcept;
    void destroyWidgets() noexcept;
    std::vector<BlueTriangleUPtr>& widgets() noexcept;
    void showNextTurnButton() noexcept;
    void hideNextTurnButton() noexcept;
    void hideArrowButtons() noexcept;
    void setPlayTime(int minutes) noexcept;
    int playTime() noexcept;
    void changeNextTurnButtonLabel(TranslateInterface::Key key) noexcept;
    void setRoomName(std::wstring roomName) noexcept;
    const std::wstring& roomName() noexcept;
    void clearAvatarSelection() noexcept;
    void saveSettings() noexcept;
    void saveLevelCompleted(int level) noexcept;
    void setLevelStatuses(int levelStatuses[]);
    int* levelStatuses() noexcept;
    bool playersJoined() noexcept;
    unsigned playerCount() noexcept;
    void setPlayerCount(unsigned playerCount) noexcept;
    double deadLineOfRunning() noexcept;
    void stopServerAt(double deadLine) noexcept;
    void setFurtherPlayersViewOnly(bool v) noexcept;
    bool furtherPlayersViewOnly() noexcept;
    void nextLevel() noexcept;
    void undoLastStep() noexcept;
    const std::wstring& profileName() noexcept;
    void setProfileName(const std::wstring& profileName) noexcept;
    unsigned viewerCount() noexcept;
    void setViewerCount(unsigned viewerCount) noexcept;
    bool isUndoCompatible() noexcept;
    void showEmotesBubble() noexcept;
    void hideEmotesBubble() noexcept;
    bool isEmotesBubbleVisible() noexcept;
    void setResolution(int index) noexcept;
    int resolution() noexcept;
    void resetMenu() noexcept;
    [[nodiscard]] bool& privacyPolicyAccepted() noexcept;
}
#endif // __APPLICATION_H__
