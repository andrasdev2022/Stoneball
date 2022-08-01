#ifndef __MULTIPLAYERCOMMON_H__
#define __MULTIPLAYERCOMMON_H__

template<typename T>
class MultiplayerCommon : public T
{
public:
    MultiplayerCommon();
    void recalcEveryXY(MultiplayerWorld* world) noexcept override;

    void recalcOtherObjectsXY(MultiplayerWorld* world) noexcept;

    uint8_t viewOnly() const noexcept override {return viewOnly_;}
    void setViewOnly(uint8_t viewOnly) noexcept override {viewOnly_ = viewOnly;}

    virtual bool onGameObjectChange(GameObject*, UDPMessageData::Fields) noexcept override;

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
    uint32_t firstYellowCardId_{0xffffffff};
    uint32_t firstGreenCardId_{0xffffffff};
};

#include "MultiplayerCommon.cpp"
#endif // __MULTIPLAYERCOMMON_H__
