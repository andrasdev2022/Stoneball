#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "ObserverTouchEvent.h"
#include "GameObject.h"
#include "TranslateInterface.h"

class World;
using WorldUPtr = std::unique_ptr<World>;

class Button final : public GameObject, public  ObserverTouchEvent, public TouchEventListener {
public:
    Button(double x, double y, GameObjectInterface::Type type) noexcept;
    bool onTouchEvent(int action, double x, double y) noexcept override;
    bool notify(ObserverTouchEventSubject* subject) noexcept override;
    void setWorldPtr(WorldUPtr& world) noexcept {this->world_ = world.get();}

    void update(double now) noexcept override;
    void draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    const GameObjectDescriptor& god() const noexcept override;
    bool flipped() const noexcept override { return direction_.x < 0; }

    bool handleArrowButtons(int action) noexcept;
    void setLabel(TranslateInterface::Key key) noexcept {label_ = key;}
    void setType(GameObjectInterface::Type type) noexcept;

    // SerializeInterface, nothing to do
    bool addData(Serialize*) override {return false;}
    //const uint8_t* buildFrom(UDPMessageData*, const uint8_t*) override {return nullptr;}

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return GameObject::id();}
protected:

private:
    void hideCards() noexcept;
    bool checkWorldPtr() noexcept;
    bool viewOnlyEmotesButton() noexcept;

    World* world_{};
    bool pushed_{};
    TranslateInterface::Key label_{TranslateInterface::Key::LAST_ONE};
};

using ButtonUPtr = std::unique_ptr<Button>;

#endif // __BUTTON_H__
