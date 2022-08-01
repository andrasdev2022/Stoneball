#ifndef __UDPSERVERSNAPSHOT_H__
#define __UDPSERVERSNAPSHOT_H__

#include "UDP.h"
#include "Serialize.h"
#include <list>
#include <map>
#include "UDPMessageData.h"
#include <vector>
#include "GameObjectInterface.h"

class MultiplayerWorld;

class UDPServerSnapshot;
using UDPServerSnapshotUPtr = std::unique_ptr<UDPServerSnapshot>;

class UDPServerSnapshot : public UDP::Message {
    UDPServerSnapshot() noexcept;
    friend class UDPMessageFactory;
public:
    // UDP::Message
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::SERVER_SNAPSHOT;}
    const unsigned char* payload() noexcept override;

    bool addData(const MultiplayerWorld* world, uint32_t clientId, double now, uint16_t moveHistorySize, uint32_t activeKnightId,
                 unsigned char activeTeamId, uint32_t score, uint16_t elapsedGameTime1, uint16_t elapsedGameTime2, uint16_t elapsedGameTime3,
                 uint16_t elapsedGameTime4, uint8_t playTime, uint8_t winnerTeam, uint8_t viewOnly, uint8_t avatar,
                 const std::vector<uint8_t>& quitters, std::vector<std::wstring>&& teamNames, uint16_t viewerCount) noexcept;

    UDPServerSnapshot* serverSnapshot() noexcept override {return this;}

    const std::list<UDPMessageData>& knights() const noexcept {return knights_;}
    const std::list<UDPMessageData>& particles() const noexcept {return particles_;}
    const std::list<UDPMessageData>& rollingStones() const noexcept {return rollingStones_;}
    const std::list<UDPMessageData>& otherElements() const noexcept {return otherElements_;}
    const std::list<UDPMessageData>& rpgBgElements() const noexcept {return rpgBgElements_;}

    uint32_t clientId() const {return clientId_;}
    bool updateClientId(uint32_t clientId) noexcept;
    uint32_t tick() const noexcept {return tick_;}
    double timeStamp() const noexcept {return timeStamp_;}
    uint16_t moveHistorySize() const noexcept {return moveHistorySize_;}
    uint32_t activeKnightId() const noexcept {return activeKnightId_;}
    unsigned char activeTeamId() const noexcept {return activeTeamId_;}
    uint32_t score() const noexcept {return score_;}
    uint16_t elapsedGameTime1() const noexcept {return elapsedGameTime1_;}
    uint16_t elapsedGameTime2() const noexcept {return elapsedGameTime2_;}
    uint16_t elapsedGameTime3() const noexcept {return elapsedGameTime3_;}
    uint16_t elapsedGameTime4() const noexcept {return elapsedGameTime4_;}
    uint8_t playTime() const noexcept {return playTime_;}
    uint8_t winnerTeam() const noexcept {return winnerTeam_;}
    uint8_t viewOnly() const noexcept {return viewOnly_;}
    bool updateViewOnly(uint8_t viewOnly) noexcept;
    uint8_t avatar() const noexcept {return avatar_;}
    bool updateAvatar(uint8_t avatar) noexcept;
    const std::vector<uint8_t>& quitters() const noexcept{return quitters_;}
    const std::vector<std::wstring>& teamNames() const noexcept {return teamNames_;}
    uint16_t viewerCount() const noexcept {return viewerCount_;}

private:
    template<typename T>
    void serializeEntityMap(const std::map<uint32_t, T>& map) noexcept;

    uint32_t tick_{};
    uint32_t clientId_;
    double timeStamp_{};
    Serialize serialize_;

    // buildFrom stores data in here
    std::list<UDPMessageData> knights_;
    std::list<UDPMessageData> particles_;
    std::list<UDPMessageData> rollingStones_;
    std::list<UDPMessageData> otherElements_;
    std::list<UDPMessageData> rpgBgElements_;
    size_t clientIdOffset_{Serialize::SERIALIZE_ERROR};
    uint16_t moveHistorySize_{};
    uint32_t activeKnightId_{};
    uint8_t activeTeamId_{};
    uint32_t score_{};
    uint16_t elapsedGameTime1_{};
    uint16_t elapsedGameTime2_{};
    uint16_t elapsedGameTime3_{};
    uint16_t elapsedGameTime4_{};
    uint8_t playTime_{};
    uint8_t winnerTeam_{255};
    uint8_t viewOnly_{};
    size_t viewOnlyOffset_{Serialize::SERIALIZE_ERROR};
    uint8_t avatar_{}; // e.g.: GameObject::Type::GOT_KNIGHT_BLUE
    size_t avatarOffset_{Serialize::SERIALIZE_ERROR};
    std::vector<uint8_t> quitters_;
    std::vector<std::wstring> teamNames_;
    uint16_t viewerCount_{};
};

template<typename T>
void UDPServerSnapshot::serializeEntityMap(const std::map<uint32_t, T>& entityMap) noexcept {
    // the dead stonball must be sent, because the client cannot play the goal effect without that gameobject
    uint16_t size = 0;
    for(const auto& p : entityMap) {
        if(!p.second->isDead() || p.second->type() == GameObjectInterface::Type::GOT_ROLLINGSTONE) {
            ++size;
        }
    }

    serialize_.add(size);
    for(const auto& p : entityMap) {
        if(!p.second->isDead() || p.second->type() == GameObjectInterface::Type::GOT_ROLLINGSTONE) {
            p.second->addData(&serialize_);
        }
    }
}

#endif // __UDPSERVERSNAPSHOT_H__
