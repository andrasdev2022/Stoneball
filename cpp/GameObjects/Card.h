#ifndef __CARD_H__
#define __CARD_H__

#include "BlueTriangle.h"

class Card : public BlueTriangle, public TouchEventListener
{
public:
    Card(double x, double bottomY, GameObject::Type type, double coolDownTime) noexcept;

    // GameObject
    void draw(Screen* screen) noexcept override;
    void update(double) noexcept override;

    // from BlueTriangle
    const Card* card() const noexcept override {return this;}
    Card* card() noexcept override {return this;}

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;

    bool isTouchValid(int action, double x, double y) noexcept;

    virtual bool onClicked() noexcept;

    virtual bool visible() const noexcept {return visible_;}
    virtual void setVisible(bool v) noexcept;
    virtual void show() noexcept {visible_ = true;}
    virtual void hide() noexcept;

    [[nodiscard]] virtual uint8_t teamIndex() const noexcept {return teamIndex_;}
    virtual void setTeamIndex(uint8_t teamIndex) noexcept {teamIndex_ = teamIndex;}
    [[nodiscard]] virtual uint8_t amount() const noexcept {return amount_;}
    virtual void setAmount(uint8_t amount) noexcept {amount_ = amount;}
    [[nodiscard]] virtual bool active() const noexcept {return active_;}
    virtual void setActive(bool active) noexcept {active_ = active;}

    [[nodiscard]] virtual double coolDown() const noexcept{return coolDown_;}
    virtual void resetCoolDown() noexcept {coolDown_ = coolDownTime_; if(amount_ != 0) --amount_;}

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

    // for SnapshotSyncer
    virtual void setCoolDown(double coolDownTime) noexcept {coolDown_ = coolDownTime;}

protected:
    virtual void setBitmapDisabled() noexcept {}
    virtual void setBitmapNormal() noexcept {}

private:
    void drawAmount(Screen* screen) noexcept;
    void drawCoolDownTime(Screen* screen) noexcept;

    const double coolDownTime_;
    bool visible_{};
    uint8_t teamIndex_{255};
    uint8_t amount_{255};
    bool active_{};
    double coolDown_{coolDownTime_};
    double hideTimer_{};
};

using CardUPtr = std::unique_ptr<Card>;

#endif // __CARD_H__
