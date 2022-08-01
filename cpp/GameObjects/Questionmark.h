#ifndef __QUESTIONMARK_H__
#define __QUESTIONMARK_H__

#include "BlueTriangle.h"

class Questionmark : public BlueTriangle
{
public:
    Questionmark(double delayMs, double timeoutMs, uint32_t linkedGameObjectId);
    BlueTriangle* copy() const noexcept override;

    virtual bool visible() const noexcept;
    virtual void setVisible(bool visible) noexcept;
    virtual void setLinkedGameObjectId(uint32_t linkedGameObjectId) noexcept {linkedGameObjectId_ = linkedGameObjectId;}

    // GameObject
    void update(double now) noexcept override;
    void draw(Screen* screen) noexcept override;
    double z() const noexcept override;

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

    Questionmark* questionmark() noexcept override {return this;}
private:
    double delayMs_;
    double timeoutMs_;
    double startTime_{};
    bool latestVisiblity_{};
    uint32_t linkedGameObjectId_;
};

#endif // __QUESTIONMARK_H__
