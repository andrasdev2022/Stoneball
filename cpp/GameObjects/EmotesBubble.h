#ifndef __EMOTESBUBBLE_H__
#define __EMOTESBUBBLE_H__

#include "ObserverTouchEvent.h"
#include "GameObject.h"
#include "GameObjectDescriptor.h"

class World;
using WorldUPtr = std::unique_ptr<World>;

class EmotesBubble final : public GameObject, public  ObserverTouchEvent, public TouchEventListener {
public:
    EmotesBubble(double x, double y) noexcept;
    bool onTouchEvent(int action, double x, double y) noexcept override;
    bool notify(ObserverTouchEventSubject* subject) noexcept override;
    void setWorldPtr(WorldUPtr& world) noexcept {this->world_ = world.get();}

    void update(double now) noexcept override;
    void draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    const GameObjectDescriptor& god() const noexcept override;

    bool visible() const noexcept {return visible_;}
    void setVisible(bool visible) noexcept;

    // SerializeInterface, nothing to do
    bool addData(Serialize*) override {return false;}

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return GameObject::id();}
protected:

private:
    int getSelectedEmoteIndex(int x, int y);
    void updateGOD() noexcept;

    World* world_{};
    bool pushed_{};
    bool visible_{};
    double sentTime_{};
    GameObjectDescriptor god_;
};

using EmotesBubbleUPtr = std::unique_ptr<EmotesBubble>;

#endif // __EMOTESBUBBLE_H__
