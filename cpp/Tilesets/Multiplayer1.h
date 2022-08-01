#ifndef __MULTIPLAYER1_H__
#define __MULTIPLAYER1_H__

#include "PlayWithFriend1.h"
#include "MultiplayerCommon.h"

class Multiplayer1 : public MultiplayerCommon<PlayWithFriend1>
{
public:
    Multiplayer1();

    Multiplayer1* multiplayer1() noexcept override {return this;}
#if 0
    void recalcEveryXY(MultiplayerWorld* world) noexcept override;
    virtual bool onGameObjectChange(GameObject*, UDPMessageData::Fields) noexcept override;
    uint8_t viewOnly() const noexcept override {return viewOnly_;}
    void setViewOnly(uint8_t viewOnly) noexcept override {viewOnly_ = viewOnly;}


    void recalcOtherObjectsXY(MultiplayerWorld* world) noexcept;

private:
    void recalcKnightsXY(MultiplayerWorld* world) noexcept;
    void recalcKnightX(GameObject* gameObject) noexcept;
    void recalcKnightY(GameObject* gameObject) noexcept;

    void recalcRollingStonesXY(MultiplayerWorld* world) noexcept;
    void recalcRollingStoneX(GameObject* gameObject) noexcept;
    void recalcRollingStoneY(GameObject* gameObject) noexcept;

    void recalcHolesXY(MultiplayerWorld* world) noexcept;
    void recalcHoleX(GameObject* gameObject) noexcept;
    void recalcHoleY(GameObject* gameObject) noexcept;

    void recalcOtherOX(GameObject* gameObject) noexcept;
    void recalcOtherOY(GameObject* gameObject) noexcept;

    uint8_t viewOnly_{};
    uint32_t firstRedCardId_{0xffffffff};
    uint32_t firstGrayCardId_{0xffffffff};
#endif
};

#endif // __MULTIPLAYER1_H__
