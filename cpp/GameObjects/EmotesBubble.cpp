#include "EmotesBubble.h"
#include "stoneball.h"
#include "ObserverTouchEventSubject.h"
#include "Application.h"
#include "Tileset.h"
#include "World.h"
#include "MultiplayerWorld.h"
#include "MediaData.h"

EmotesBubble::EmotesBubble(double x, double bottomY) noexcept
: GameObject(x, bottomY, GameObject::Type::GOT_EMOTES_BUBBLE)
, world_(nullptr)
{
    direction_ = Vector{0., 0.};
    god_ = GameObjectDescriptor::find(type(), State::Type::ST_IDLE);
    init();
}

bool EmotesBubble::notify(ObserverTouchEventSubject* subject) noexcept {
    return onTouchEvent(subject->action(), subject->x(), subject->y());
}

bool EmotesBubble::onTouchEvent(int action, double x, double y) noexcept {
    if(action == ACTION_MOVE || !visible()) return false;

    if(Collision::insideRect(x, y ,x_, y_, width_, height_)) {
        if(action == ACTION_DOWN) {
            pushed_ = true;
            //return true;
        } else if(action == ACTION_UP && pushed_) {
            pushed_ = false;
            // Which one was selected?
            int i = getSelectedEmoteIndex(x, y);
            // message to server
            if(world_ && (now_ms() - sentTime_ > 10'000)) {
                if(!Application::game() || !Application::game()->worldListSize()) return false;
                setWorldPtr(Application::latestWorld());
                world_->multiplayer()->onEmoteBubbleSelected(i);
                sentTime_ = now_ms();
            }
            Application::hideEmotesBubble();
        }
        return true;
    } else if(pushed_) {
        pushed_ = false;
    }
    return false;
}

void EmotesBubble::updateGOD() noexcept {
    if(now_ms() - sentTime_ > 10'000) {
        god_.setSkinMin(NATIVE_BITMAP_EMOTES_BUBBLE);
        god_.setSkinMax(NATIVE_BITMAP_EMOTES_BUBBLE);
        god_.setSkinStart(NATIVE_BITMAP_EMOTES_BUBBLE);
    } else {
        god_.setSkinMin(NATIVE_BITMAP_EMOTES_BUBBLE_DISABLED);
        god_.setSkinMax(NATIVE_BITMAP_EMOTES_BUBBLE_DISABLED);
        god_.setSkinStart(NATIVE_BITMAP_EMOTES_BUBBLE_DISABLED);
    }
    skinMin_ = god_.skinMin();
    skinMax_ = god_.skinMax();
    frameIndex_ = god_.skinStart();
}

void EmotesBubble::update(double now) noexcept {
    if(!visible()) return;
    updateGOD();
    setX(screenWidth() - 172_ZRD);
    setY(screenHeight() - 302_ZRD);
    updateFrame(now, skinMin_, skinMax_);
}

void EmotesBubble::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!visible()) return;
    GameObject::draw(info, pixels);
}

const GameObjectDescriptor& EmotesBubble::god() const noexcept {
    return god_;
}

void EmotesBubble::setVisible(bool visible) noexcept {
    visible_ = visible;
}

int EmotesBubble::getSelectedEmoteIndex(int x, int y) {
    int xd = x - x_;
    int yd = y - y_;
    int eW = width_ / 3;
    int eH = height_ / 2;
    return (int)(yd / eH) * 3 + (int)(xd / eW);
}
