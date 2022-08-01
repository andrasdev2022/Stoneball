#ifndef __GAMEINTERFACE_H__
#define __GAMEINTERFACE_H__

#include <memory>
#include <map>
#include <vector>

#include "Defines.h"
#include "TranslateInterface.h"

class Button;
class World;
using WorldUPtr = std::unique_ptr<World>;
using ButtonUPtr = std::unique_ptr<Button>;
class SingleplayerGame;
class WorldTouchEvent;
using WorldTouchEventUPtr = std::unique_ptr<WorldTouchEvent>;
class EmotesBubble;

class GameInterface {
public:
    virtual ~GameInterface() = default;

    virtual void updateWorld(double now) noexcept = 0;
    virtual void drawWorld(AndroidBitmapInfo* info, void* pixels) noexcept = 0;
    virtual void wakeUp() noexcept = 0;
    [[nodiscard]] virtual WorldUPtr& latestWorld() noexcept = 0;
    [[nodiscard]] virtual std::map<double, WorldUPtr>& worldList() noexcept = 0;
    [[nodiscard]] virtual size_t worldListSize() noexcept = 0;
    [[nodiscard]] virtual std::vector<Button*>& buttons() noexcept = 0;
    virtual void initButtons() noexcept = 0;
    virtual void changeButtonsAccordingToPlaymode() noexcept = 0;
    [[nodiscard]] virtual Button* nextTurnButton() noexcept = 0;
    virtual SingleplayerGame* singleplayerGame() noexcept {return nullptr;}
    virtual void updateWorldOfButtons(WorldUPtr& world) noexcept = 0;
    virtual void resetWorldTouchEvent() noexcept = 0;
    virtual WorldTouchEvent* worldTouchEvent() noexcept = 0;
    virtual bool showNextTurnButton() noexcept = 0;
    virtual Button* hideNextTurnButton() noexcept = 0;
    virtual void changeNextTurnButtonLabel(TranslateInterface::Key key) noexcept = 0;
    virtual void hideArrowButtons() noexcept = 0;

    [[nodiscard]] virtual Button* arrowButtonRight() noexcept = 0;
    [[nodiscard]] virtual Button* arrowButtonUp() noexcept = 0;
    [[nodiscard]] virtual Button* arrowButtonDown() noexcept = 0;
    [[nodiscard]] virtual Button* arrowButtonLeft() noexcept = 0;
    [[nodiscard]] virtual EmotesBubble* emotesBubble() noexcept = 0;

};

using GameInterfaceUPtr = std::unique_ptr<GameInterface>;

#endif // __GAMEINTERFACE_H__
