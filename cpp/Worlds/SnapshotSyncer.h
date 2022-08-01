#ifndef __SNAPSHOTSYNCER_H__
#define __SNAPSHOTSYNCER_H__

#include "UDPServerDeltaSnapshot.h"
#include <map>
#include <set>
#include <algorithm>
#include "MultiplayerWorld.h"
#include <functional>

class GameObject;
class Knight;

class SnapshotSyncer final
{
public:
    using DeltaSPostCallback = std::function<bool(GameObject*, UDPMessageData::Fields)>;
    SnapshotSyncer(MultiplayerWorld* world) : world_(world) {}
    void sync(const UDPServerDeltaSnapshot* snapshot, DeltaSPostCallback d) noexcept;
    void sync(UDPServerSnapshot* snapshot) noexcept(false);

private:
    void sync(const UDPServerDeltaSnapshot::MessageData& msgData, DeltaSPostCallback d) noexcept;
    void syncMoveHistory(uint16_t moveHistorySize) noexcept;
    void syncActiveKnightId(uint32_t activeKnightId) noexcept;
    void syncActiveTeamId(unsigned char activeTeamId) noexcept;
    void syncScore(uint32_t score) noexcept;
    void syncElapsedGameTime(uint8_t idx, uint16_t value) noexcept;
    void syncWinnerTeam(uint8_t team) noexcept;
    void syncViewOnly(uint8_t viewOnly) noexcept;
    void syncAvatar(uint8_t avatar) noexcept;
    void createGameObject(const UDPServerDeltaSnapshot::MessageData& msgData) noexcept;
    void removeGameObject() noexcept;
    void syncPlayTime(uint8_t playTime) noexcept;
    void syncQuitters(const std::vector<uint8_t>& quitters) const noexcept;
    void syncTeamNames(const std::vector<std::wstring>& teamNames) noexcept;
    void syncViewerCount(uint16_t viewerCount) noexcept;

    virtual void initHero(const UDPMessageData& msgData) noexcept;
    virtual void updateHero(KnightUPtr& knight, const UDPMessageData& msgData) noexcept;
    virtual void initParticle(const UDPMessageData& msgData) noexcept;
    virtual void updateParticle(ParticleUPtr& particle, const UDPMessageData& msgData) noexcept;
    virtual void initRollingStone(const UDPMessageData& msgData) noexcept;
    virtual void updateRollingStone(RollingStoneUPtr& rollingStone, const UDPMessageData& msgData) noexcept;
    virtual void initOtherElement(const UDPMessageData& msgData) noexcept;
    virtual void updateOtherElement(BlueTriangleUPtr& otherElement, const UDPMessageData& msgData) noexcept;
    virtual void initRPGBgElement(const UDPMessageData& msgData) noexcept;
    virtual void updateRPGBgElement(RPGBgElementUPtr& otherElement, const UDPMessageData& msgData) noexcept;
    virtual void updateGameObject(GameObject* gameObject, const UDPMessageData& msgData) noexcept;

    typedef void (SnapshotSyncer::*InitFuncPtr)(const UDPMessageData& msgData) noexcept;

    template<typename UpdateFunc, typename WorldMap>
    void sync3(const std::list<UDPMessageData>& messageDatas, InitFuncPtr initFunc,
               UpdateFunc updateFunc, WorldMap& worldMap, char goType);

    UDPMessageData fullSnapshot_;
    GameObject* gameObject_{};
    Knight* knight_{};
    RollingStone* stone_{};
    BlueTriangle* blueTriangle_{};
    MultiplayerWorld* world_;
};

template<typename UpdateFunc, typename WorldMap>
void SnapshotSyncer::sync3(const std::list<UDPMessageData>& messageDatas,
                           InitFuncPtr initFunc, UpdateFunc updateFunc, WorldMap& worldMap, char goType) {
    for(auto& msgData : messageDatas) {
        if(msgData.type != goType) {
            throw std::runtime_error(std::string("MultiplayerWorld::sync ") + std::to_string(goType));
        }
        auto it = worldMap.find(msgData.id);
        if(it == worldMap.end() && !msgData.dead) {
            (this->*initFunc)(msgData);
        } else {
            if(msgData.dead && it != worldMap.end()) {
                worldMap.erase(it);
                continue;
            }
            if(it != worldMap.end()) {
                (this->*updateFunc)(it->second, msgData);
            }
        }
    }
}

#endif // __SNAPSHOTSYNCER_H__
