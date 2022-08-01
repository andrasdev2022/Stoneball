#ifndef __EMOTEFIGURE_H__
#define __EMOTEFIGURE_H__

#include "BlueTriangle.h"

class EmoteFigure : public BlueTriangle
{
public:
    EmoteFigure(double delayMs, double timeoutMs, uint32_t emoteId, uint8_t avatar);
    BlueTriangle* copy() const noexcept override;

    virtual bool visible() const noexcept;
    virtual void setVisible(bool visible) noexcept;

    // GameObject
    void update(double now) noexcept override;
    void draw(Screen* screen) noexcept override;
    double z() const noexcept override;
    const GameObjectDescriptor& god() const noexcept override;

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

    EmoteFigure* emoteFigure() noexcept override {return this;}
    void setEmoteId(uint32_t emoteId);

    uint8_t avatar() const noexcept {return avatar_;}
    void setAvatar(uint8_t avatar) noexcept {avatar_ = avatar;}

private:
    void changeDescriptor() noexcept;

    double delayMs_;
    double timeoutMs_;
    double startTime_{};
    bool latestVisiblity_{};
    GameObjectDescriptor descriptor_;
    uint32_t emoteId_{0xff};
    uint8_t avatar_{};
};

using EmoteFigureUPtr = std::unique_ptr<EmoteFigure>;

#endif // __EMOTEFIGURE_H__
