#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <utility>
#include "android.h"
//#include "Global.h"
#include "TouchEventListener.h"
#include "ButtonEventListener.h"
#include "CollisionData.h"
#include <memory>
#include "Vector.h"
#include "GameObjectInterface.h"
#include "CommonUtil.h"
#include "SerializeInterface.h"

class GameObjectDescriptor;
class MediaData;
class CollisionData;
class Screen;

class GameObject : public GameObjectInterface, public ButtonEventListener, public SerializeInterface {
public:
    GameObject(double x, double bottomY, Type type) noexcept;
    GameObject(const GameObject& g) noexcept = default;
    virtual void init() noexcept;

    virtual void update(double now) = 0;
    virtual void draw(AndroidBitmapInfo* info, void* pixels) noexcept;
    virtual void draw(Screen* screen) noexcept;
    virtual const MediaData* frameData() const noexcept;
    virtual bool flipped() const noexcept {return false;}
    virtual void setFlipped(bool) noexcept {}

    virtual double x() const noexcept {return x_;}
    virtual void setX(double x) noexcept {x_ = x;}
    virtual double y() const noexcept {return y_;}
    virtual void setY(double y) noexcept {y_ = y;}
    virtual double z() const noexcept; // for RPG view
    virtual Type type() const noexcept {return type_;}
    virtual int width() const noexcept {return width_;}
    virtual int height() const noexcept {return height_;}
    virtual void setWidth(int width) noexcept {width_ = width;}
    virtual void setHeight(int height) noexcept {height_ = height;}
    virtual void yCorrectionByGravity(double newY, bool collision) noexcept;
    virtual double velocity() const noexcept {return velocity_;}
    virtual void setVelocity(double v) noexcept {velocity_ = v;}

    virtual void die() noexcept {}
    virtual bool isDead() const noexcept {return false;}
    virtual void hurt(int damagePerFrame) noexcept {hitPoints_ -= damagePerFrame;}
    virtual bool isHurted() const noexcept {return false;}

    virtual const GameObjectDescriptor& god() const = 0;
    virtual int frameIndex() const noexcept {return frameIndex_;}
    virtual void setFrameIndex(int frameIndex) noexcept {frameIndex_ = frameIndex;}

    virtual int damagePerFrame() const noexcept {return damagePerFrame_;}
    virtual void setDamagePerFrame(int v) noexcept {damagePerFrame_ = v;}
    virtual int newDamagePoints(double now) noexcept {Unused(0, now); return 0;}

    virtual int hitPoints() const noexcept {return hitPoints_;}
    virtual int maxHitPoints() const noexcept {return maxHitPoints_;}
    virtual void setHitPoints(int hitPoints, int maxHitPoints) noexcept {hitPoints_ = hitPoints; maxHitPoints_ = maxHitPoints;}
    virtual int skinMax() const noexcept {return skinMax_;}
    virtual void setSkinMax(int skinMax) noexcept {skinMax_ = skinMax;}
    virtual int skinMin() const noexcept {return skinMin_;}
    virtual void setSkinMin(int skinMin) noexcept {skinMin_ = skinMin;}
    virtual double contentBottom() const noexcept;
    virtual void setTransparency(int v) noexcept {transparency_ = v;}
    virtual void wakeUp(double now) noexcept;
    virtual void setStateChangeAvailable(bool available) noexcept {stateChangeAvailable_ = available;}
    virtual bool stateChangeAvailable() noexcept {return stateChangeAvailable_;}
    virtual bool respawn() noexcept {return false;}
    virtual double directionX() const noexcept {return direction_.x;}
    virtual double directionY() const noexcept {return direction_.y;}
    virtual VectorD direction() const noexcept {return direction_;}
    virtual void setDirectionX(double directionX) noexcept {direction_.x = directionX;}
    virtual void setDirectionY(double directionY) noexcept {direction_.y = directionY;}
    virtual CollisionData* collisionData() const noexcept {return collisionData_.get();}
    virtual void setCollisionData(CollisionData* collisionData) noexcept {collisionData_.reset(collisionData);}

    uint32_t id() const noexcept {return id_;}
    void setId(uint32_t v) noexcept {id_ = v;}

    static std::string str(Type t) noexcept;
    static std::string str(Event t) noexcept;

    // ButtonEventListener
    bool onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept override {Unused(0, sender, buttonState); return false;}

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

protected:
    //virtual void copyImg(const uint16_t* srcBitmap, int srcX, int srcY, int srcWidth, int srcHeight,
    //                     AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const;
    virtual void copyImg(const MediaData* frameData, AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept;
    virtual void copyImgRareMatrix(const MediaData* frameData, AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept;
    virtual void drawHPBar(AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept;
    virtual void drawHPBar(Screen* screen) noexcept;
    virtual void drawColliderShape(AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept;
    virtual void drawColliderShape(Screen* screen) noexcept;
    virtual void updateFrame(double now, int minFrameIndex, int maxFrameIndex) noexcept;
    virtual void walkOrMove(double now) noexcept;
    virtual void jump(double now, double jumpTime, double maxHeightOfJump, double correction, double& startTimeOfJump, double& yPosAtStart) noexcept;
    virtual void useBorderRestrictions(const int bgWidth, const int bgHeight) noexcept;

    double x_;
    double y_;
    int width_;
    int height_;
    double velocity_;  // pixel/millis
    double animTimeStep_; // millis
    double lastAnimFrameTmps{0}; // millis
    Vector<double> direction_;
    double lastUpdateTmps{0}; // last update timestamp in millis
    int frameIndex_{-1}; // e.g. index on the chibi1.png
    int skinMin_;
    int skinMax_;
    Type type_;

    /* attack */
    // favorite target
    // damage type: single; area splash
    // targets: ground; air
    // movement speed:
    int damagePerFrame_{-1};
    /* defense */
    // health points: how close the character to death. HP.
    int hitPoints_{-1};
    int maxHitPoints_{-1};
    int transparency_{0};
    uint32_t id_;
    bool stateChangeAvailable_;
    size_t skipEveryNthLine{}; // for drawing
    mutable size_t lineCounter{}; // for drawing
    std::shared_ptr<CollisionData> collisionData_;
};

using GameObjectUPtr = std::unique_ptr<GameObject>;

#endif // __GAMEOBJECT_H__
