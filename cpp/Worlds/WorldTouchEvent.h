#ifndef __WORLDTOUCHEVENT_H__
#define __WORLDTOUCHEVENT_H__

#include "ObserverTouchEvent.h"
#include <memory>
#include "IdGenerator.h"

class World;
using WorldUPtr = std::unique_ptr<World>;
class Knight;

class WorldTouchEvent : public ObserverTouchEvent {
public:
    WorldTouchEvent() : id_(IdGenerator::generateId()) {}
    bool notify(ObserverTouchEventSubject* subject) noexcept override;
    void setWorldPtr(WorldUPtr& world) noexcept {world_ = world.get();}

    bool handleClickOnBalls(wchar_t tileContent, int tileX, int tileY) noexcept;
    void teleport(Knight* knight, int tileX, int tileY) noexcept;
    void popQuestionmarksAboveKnights() noexcept;

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return id_;}

private:
    bool handleClickOnKnight(ObserverTouchEventSubject* subject) noexcept;
    void nonActiveKnightsSitDown() noexcept;
    bool forwardTouchEventForDeckOfCards(ObserverTouchEventSubject* subject) noexcept;
    bool forwardTouchEventForCards(ObserverTouchEventSubject* subject) noexcept;
    bool testTargetTile(int tileX, int tileY) noexcept;
    bool handleClickOnBalls(ObserverTouchEventSubject* subject) noexcept;

    World* world_{nullptr};
    uint32_t id_;
    static constexpr int NOT_GRABBED = 0;
    static constexpr int GRABBED = 1;
    static constexpr int GRABBED_AND_MOVING = 2;
    int grabbed_{NOT_GRABBED};
    double grabbedX_{};
    double grabbedY_{};
    int deltaX_{};
    int deltaY_{};
};

using WorldTouchEventUPtr = std::unique_ptr<WorldTouchEvent>;

#endif // __WORLDTOUCHEVENT_H__
