#ifndef __UDPSERVERDELTASNAPSHOT_H__
#define __UDPSERVERDELTASNAPSHOT_H__

#include "UDP.h"
#include "Serialize.h"
#include "UDPServerSnapshot.h"
#include <list>
#include <unordered_map>
#include "Math_.h"
#include <iostream>

class Knight;
class MultiplayerWorld;
class GameObject;

class UDPServerDeltaSnapshot : public UDP::Message {
    UDPServerDeltaSnapshot() noexcept = default;
    UDPServerDeltaSnapshot(const UDPServerSnapshot* oldOne, const UDPServerSnapshot* newOne, double now) noexcept;
    friend class UDPMessageFactory;
public:
    struct MessageData {
        UDPMessageData::Fields field;
        union { // copy of UDPMessageData
            uint32_t id; // gameobjectid
            double x;
            double contentBottom;
            uint16_t frameIndex;
            uint16_t skinMin;
            uint16_t skinMax;
            unsigned char state; // idle, attack, run, etc.
            unsigned char gameObjectType;  // e.g.: GameObject::Type::GOT_KNIGHT_BLUE
            int32_t hitPoints;
            int32_t maxHitPoints;
            int32_t linkedGameObjectId;
            unsigned char dead; // 0 = no, 1 = yes
            unsigned char type; // K, Z, V, G, B, P, S, R, O
            unsigned char flipped;
            unsigned char color; // filled by rolling stone
            unsigned char messageKey; // e.g. from messagebox
            unsigned char visible; // filled by cards
            uint8_t teamIndex; // filled by cards
            uint8_t amount; // filled by cards
            double coolDown; // filled by cards
            uint32_t scores; // filled by MatchResults
            uint64_t elapsedTimes; // filled by MatchResults
            uint8_t teamCount;  // filled by MatchResults
            uint8_t looserTeam{};  // filled by MatchResults
            uint8_t showCountDown;  // filled by Messagebox1
            double spareTime;  // filled by Messagebox1
        };
        std::wstring string;
    };
    // UDP::Message
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::SERVER_DELTA_SNAPSHOT;}
    const unsigned char* payload() noexcept override;

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
    uint8_t winnerTeam() const noexcept {return winnerTeam_;}
    const std::vector<uint8_t>& quitters() const noexcept{return quitters_;}
    const std::vector<std::wstring>& teamNames() const noexcept {return teamNames_;}
    uint16_t viewerCount() const noexcept {return viewerCount_;}

    bool isMoveHistorySizePresent() const noexcept;
    bool isActiveKnightIdPresent() const noexcept;
    bool isActiveTeamIdPresent() const noexcept;
    bool isScorePresent() const noexcept;
    bool isElapsedGameTime1Present() const noexcept;
    bool isElapsedGameTime2Present() const noexcept;
    bool isElapsedGameTime3Present() const noexcept;
    bool isElapsedGameTime4Present() const noexcept;
    bool isWinnerTeamPresent() const noexcept;
    bool isQuittersPresent() const noexcept;
    bool isTeamNamesPresent() const noexcept;
    bool isViewerCountPresent() const noexcept;

    const std::list<MessageData>& builtData() const {return builtData_;}
    using MsgMap = std::unordered_map<uint32_t, UDPMessageData>;
    using MsgMapPair = std::pair<uint32_t, UDPMessageData>;

    UDPServerDeltaSnapshot* serverDeltaSnapshot() noexcept override {return this;}
private:

    void addDiff(unsigned char type, uint32_t id, const MsgMapPair& pair, const UDPMessageData& newData);
    void compareSnapshots(const unsigned char type, const std::list<UDPMessageData>& oldList, const std::list<UDPMessageData>& newList) noexcept;

    uint32_t tick_{};
    uint32_t clientId_{};
    double timeStamp_{};
    Serialize serialize_;

    // buildFrom stores data in here
    std::list<MessageData> builtData_;

    void serialize(Serialize& item, unsigned char type, uint32_t id);

    template<typename T>
    void serialize(Serialize& item, unsigned char type, uint32_t id, UDPMessageData::Fields fieldId, T x1, T x2);

    size_t diffListSizePos_;
    unsigned char diffListSize_;
    size_t clientIdOffset_{Serialize::SERIALIZE_ERROR};

    // there is a bitfield in order to show the presence of the fields below
    enum PresenceBits {
        PB_MoveHistorySize  = (1U << 0),
        PB_ActiveKnightId   = (1U << 1),
        PB_ActiveTeamId     = (1U << 2),
        PB_Score            = (1U << 3),
        PB_ElapsedGameTime1 = (1U << 4),
        PB_ElapsedGameTime2 = (1U << 5),
        PB_ElapsedGameTime3 = (1U << 6),
        PB_ElapsedGameTime4 = (1U << 7),
        PB_WinnerTeam       = (1U << 8),
        PB_Quitters         = (1U << 9),
        PB_TeamNames        = (1U << 10),
        PB_ViewerCount      = (1U << 11),
    };

    uint16_t fieldsPresence_{};
    uint16_t moveHistorySize_{};
    uint32_t activeKnightId_{};
    unsigned char activeTeamId_{};
    uint32_t score_{};
    uint16_t elapsedGameTime1_{};
    uint16_t elapsedGameTime2_{};
    uint16_t elapsedGameTime3_{};
    uint16_t elapsedGameTime4_{};
    uint8_t winnerTeam_{255};
    std::vector<uint8_t> quitters_;
    std::vector<std::wstring> teamNames_;
    uint16_t viewerCount_{};
};

using UDPServerDeltaSnapshotUPtr = std::unique_ptr<UDPServerDeltaSnapshot>;

template<typename T>
void UDPServerDeltaSnapshot::serialize(Serialize& item, unsigned char type, uint32_t id, UDPMessageData::Fields fieldId, T x1, T x2) {
    unsigned char fieldId2 = fieldId;
    //if(std::is_same<T, double>() || std::is_same<T, float>()) {
    if constexpr (std::is_floating_point<T>()) {
        if(Math::abs(x1 - x2) > 1e-5) {
            serialize(item, type, id);
            item.add(fieldId2);
            item.add(x2);
            ++diffListSize_;
        }
    } else {
        if(x1 != x2) {
            serialize(item, type, id);
            item.add(fieldId2);
            item.add(x2);
            ++diffListSize_;
        }
    }
}

template<>
void UDPServerDeltaSnapshot::serialize(Serialize& item, unsigned char type, uint32_t id, UDPMessageData::Fields fieldId, std::wstring x1, std::wstring x2);

#endif // __UDPSERVERDELTASNAPSHOT_H__
