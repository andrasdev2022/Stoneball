#include "UDPServerDeltaSnapshot.h"
#include "GameObject.h"
#include "MultiplayerWorld.h"
#include "Knight.h"
#include "Rect.h"
#include "MediaData.h"
#include "Memory.h"
#include "Algorithms.h"

using namespace Algorithms;

/*
Content order:
    1 byte: TypeId::SERVER_DELTA_SNAPSHOT
    uint32: clientId

    BLOCK BEGIN 1
        1 byte: game object character, e.g.: 'K', 'V', ...
        uint32: gameObjectId

        1 byte: diff list size
        BLOCK BEGIN 2
            1 byte: field id like Field_x
            ? bytes: new field value, size depends on field id
        BLOCK END 2
        BLOCK BEGIN 2
        ..
    BLOCK END 1
    BLOCK BEGIN 1
    ...
    BLOCK END 1
    uint32 tick
*/

namespace {
} // namespace

void UDPServerDeltaSnapshot::serialize(Serialize& item, unsigned char type, uint32_t gameObjectId) {
    if(!item.size()) {
        item.add(type); // e.g.: 'K'
        item.add(gameObjectId);
        diffListSizePos_ = item.add(diffListSize_);
    }
}

void UDPServerDeltaSnapshot::addDiff(unsigned char type, uint32_t gameObjectId, const MsgMapPair& oldPair, const UDPMessageData& newData) {
    Serialize item;
    diffListSize_ = 0;
    diffListSizePos_ = 0;
    serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_gameObjectType, oldPair.second.gameObjectType, newData.gameObjectType);
    serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_dead, oldPair.second.dead, newData.dead);
    if(!newData.dead) {
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_state, oldPair.second.state, newData.state);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_skinMin, oldPair.second.skinMin, newData.skinMin);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_skinMax, oldPair.second.skinMax, newData.skinMax);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_x, oldPair.second.x, newData.x);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_frameIndex, oldPair.second.frameIndex, newData.frameIndex);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_flipped, oldPair.second.flipped, newData.flipped);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_hitPoints, oldPair.second.hitPoints, newData.hitPoints);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_maxHitPoints, oldPair.second.maxHitPoints, newData.maxHitPoints);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_linkedGameObjectId, oldPair.second.linkedGameObjectId, newData.linkedGameObjectId);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_contentBottom, oldPair.second.contentBottom, newData.contentBottom);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_color, oldPair.second.color, newData.color);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_messageKey, oldPair.second.messageKey, newData.messageKey);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_visible, oldPair.second.visible, newData.visible);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_teamIndex, oldPair.second.teamIndex, newData.teamIndex);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_amount, oldPair.second.amount, newData.amount);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_coolDown, oldPair.second.coolDown, newData.coolDown);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_teamCount, oldPair.second.teamCount, newData.teamCount);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_looserTeam, oldPair.second.looserTeam, newData.looserTeam);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_scores, oldPair.second.scores, newData.scores);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_elapsedTimes, oldPair.second.elapsedTimes, newData.elapsedTimes);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_showCountDown, oldPair.second.showCountDown, newData.showCountDown);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_spareTime, oldPair.second.spareTime, newData.spareTime);
        serialize(item, type, gameObjectId, UDPMessageData::Fields::Field_messageString, oldPair.second.messageString, newData.messageString);
    }
    if(item.size()) {
        std::unique_ptr<unsigned char[]> content = item.get();
        content.get()[diffListSizePos_] = diffListSize_;
        serialize_.add3(content.get(), item.size());
    }
}

void UDPServerDeltaSnapshot::compareSnapshots(const unsigned char type, const std::list<UDPMessageData>& oldList,
                                              const std::list<UDPMessageData>& newList) noexcept {

    // Build a map from oldOne
    MsgMap oldTemp;
    std::for_each(oldList.begin(), oldList.end(), [&oldTemp](const UDPMessageData& data){
        oldTemp.emplace(data.id, data);
    });

    // Build a map from newOne
    MsgMap newTemp;
    std::for_each(newList.begin(), newList.end(), [&newTemp](const UDPMessageData& data){
        newTemp.emplace(data.id, data);
    });

    // Find old elements in new map
    for(const auto& oldPair : oldTemp) {
        auto it = newTemp.find(oldPair.first);
        if(it == newTemp.end()) {
            // if one is missing then ????
            // That gameobject probably died
            serialize_.add(type); // e.g.: 'K'
            serialize_.add(oldPair.first); // gameObjectId
            diffListSize_ = 1;
            serialize_.add(diffListSize_);
            unsigned char filedId = UDPMessageData::Fields::Field_INDICATOR_REMOVED;
            serialize_.add(filedId);
        } else {
            // if one is found then prepare a delta
            addDiff(type, oldPair.first, oldPair, it->second);
        }
    }

    //const UDPMessageData empty{};
    const MsgMapPair empty{};

    // Find new elements in old map
    for(const auto& newPair : newTemp) {
        auto it = oldTemp.find(newPair.first);
        if(it == oldTemp.end()) {
            // if one is missing then ????
            // A new player joined
            addDiff(type, newPair.first, empty, newPair.second);
        } else {
            // it has been added during old-elements-in-new-map comparison
        }
    }
}

bool UDPServerDeltaSnapshot::isMoveHistorySizePresent() const noexcept {return fieldsPresence_ & PB_MoveHistorySize;}
bool UDPServerDeltaSnapshot::isActiveKnightIdPresent() const noexcept {return fieldsPresence_ & PB_ActiveKnightId;}
bool UDPServerDeltaSnapshot::isActiveTeamIdPresent() const noexcept {return fieldsPresence_ & PB_ActiveTeamId;}
bool UDPServerDeltaSnapshot::isScorePresent() const noexcept {return fieldsPresence_ & PB_Score;}
bool UDPServerDeltaSnapshot::isElapsedGameTime1Present() const noexcept {return fieldsPresence_ & PB_ElapsedGameTime1;}
bool UDPServerDeltaSnapshot::isElapsedGameTime2Present() const noexcept {return fieldsPresence_ & PB_ElapsedGameTime2;}
bool UDPServerDeltaSnapshot::isElapsedGameTime3Present() const noexcept {return fieldsPresence_ & PB_ElapsedGameTime3;}
bool UDPServerDeltaSnapshot::isElapsedGameTime4Present() const noexcept {return fieldsPresence_ & PB_ElapsedGameTime4;}
bool UDPServerDeltaSnapshot::isWinnerTeamPresent() const noexcept {return fieldsPresence_ & PB_WinnerTeam;}
bool UDPServerDeltaSnapshot::isQuittersPresent() const noexcept {return fieldsPresence_ & PB_Quitters;}
bool UDPServerDeltaSnapshot::isTeamNamesPresent() const noexcept {return fieldsPresence_ & PB_TeamNames;}
bool UDPServerDeltaSnapshot::isViewerCountPresent() const noexcept {return fieldsPresence_ & PB_ViewerCount;}

UDPServerDeltaSnapshot::UDPServerDeltaSnapshot(const UDPServerSnapshot* oldOne, const UDPServerSnapshot* newOne, double now) noexcept {
    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType); // TypeId::SERVER_DELTA_SNAPSHOT

    clientIdOffset_ = serialize_.add(clientId_);
    serialize_.add(now);

    // there is a bitfield in order to show the presence of the fields below
    fieldsPresence_ = 0;
    size_t fieldPresencePosition = serialize_.add(fieldsPresence_);

    if(oldOne->moveHistorySize() != newOne->moveHistorySize()) {
        fieldsPresence_ |= PB_MoveHistorySize;
        serialize_.add(newOne->moveHistorySize());
    }
    if(oldOne->activeKnightId() != newOne->activeKnightId()) {
        fieldsPresence_ |= PB_ActiveKnightId;
        serialize_.add(newOne->activeKnightId());
    }
    if(oldOne->activeTeamId() != newOne->activeTeamId()) {
        fieldsPresence_ |= PB_ActiveTeamId;
        serialize_.add(newOne->activeTeamId());
    }
    if(oldOne->score() != newOne->score()) {
        fieldsPresence_ |= PB_Score;
        serialize_.add(newOne->score());
    }
    if(oldOne->elapsedGameTime1() != newOne->elapsedGameTime1()) {
        fieldsPresence_ |= PB_ElapsedGameTime1;
        serialize_.add(newOne->elapsedGameTime1());
    }
    if(oldOne->elapsedGameTime2() != newOne->elapsedGameTime2()) {
        fieldsPresence_ |= PB_ElapsedGameTime2;
        serialize_.add(newOne->elapsedGameTime2());
    }
    if(oldOne->elapsedGameTime3() != newOne->elapsedGameTime3()) {
        fieldsPresence_ |= PB_ElapsedGameTime3;
        serialize_.add(newOne->elapsedGameTime3());
    }
    if(oldOne->elapsedGameTime4() != newOne->elapsedGameTime4()) {
        fieldsPresence_ |= PB_ElapsedGameTime4;
        serialize_.add(newOne->elapsedGameTime4());
    }
    if(oldOne->winnerTeam() != newOne->winnerTeam()) {
        fieldsPresence_ |= PB_WinnerTeam;
        serialize_.add(newOne->winnerTeam());
    }
    if(oldOne->quitters() != newOne->quitters()) {
        fieldsPresence_ |= PB_Quitters;
        serialize_.add((uint8_t)newOne->quitters().size());
        for(auto item : newOne->quitters()) {
            serialize_.add(item);
        }
    }
    if(oldOne->teamNames() != newOne->teamNames()) {
        fieldsPresence_ |= PB_TeamNames;
        serialize_.add((uint8_t)newOne->teamNames().size());
        for(auto& item : newOne->teamNames()) {
            serialize_.addString(item);
        }
    }

    if(oldOne->viewerCount() != newOne->viewerCount()) {
        fieldsPresence_ |= PB_ViewerCount;
        serialize_.add(newOne->viewerCount());
    }

    serialize_.replace(fieldsPresence_, fieldPresencePosition);

    compareSnapshots('K', oldOne->knights(), newOne->knights());
    compareSnapshots('P', oldOne->particles(), newOne->particles());
    compareSnapshots('S', oldOne->rollingStones(), newOne->rollingStones());
    compareSnapshots('O', oldOne->otherElements(), newOne->otherElements());
    compareSnapshots('R', oldOne->rpgBgElements(), newOne->rpgBgElements());

    serialize_.add(++tick_);
}

bool UDPServerDeltaSnapshot::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        debug("ERROR: UDPServerDeltaSnapshot::buildFrom payload is null");
    }
    if(!length) {
        debug("ERROR: UDPServerDeltaSnapshot::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        debug("ERROR: UDPServerDeltaSnapshot::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    // First character of the payload is the message type
    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;

    const unsigned char* ptr = payload + 1;

    ptr = Memory::copy(clientId_, ptr);
    ptr = Memory::copy(timeStamp_, ptr);

    // there is a bitfield in order to show the presence of the fields below
    ptr = Memory::copy(fieldsPresence_, ptr);
    if(isMoveHistorySizePresent()) {
        ptr = Memory::copy(moveHistorySize_, ptr);
    }
    if(isActiveKnightIdPresent()) {
        ptr = Memory::copy(activeKnightId_, ptr);
    }
    if(isActiveTeamIdPresent()) {
        ptr = Memory::copy(activeTeamId_, ptr);
    }
    if(isScorePresent()) {
        ptr = Memory::copy(score_, ptr);
    }
    if(isElapsedGameTime1Present()) {
        ptr = Memory::copy(elapsedGameTime1_, ptr);
    }
    if(isElapsedGameTime2Present()) {
        ptr = Memory::copy(elapsedGameTime2_, ptr);
    }
    if(isElapsedGameTime3Present()) {
        ptr = Memory::copy(elapsedGameTime3_, ptr);
    }
    if(isElapsedGameTime4Present()) {
        ptr = Memory::copy(elapsedGameTime4_, ptr);
    }
    if(isWinnerTeamPresent()) {
        ptr = Memory::copy(winnerTeam_, ptr);
    }
    if(isQuittersPresent()) {
        uint8_t quittersSize{};
        uint8_t item{};
        ptr = Memory::copy(quittersSize, ptr);
        for(uint8_t i = 0; i < quittersSize; ++i) {
           ptr = Memory::copy(item, ptr);
           quitters_.push_back(item);
        }
    }
    if(isTeamNamesPresent()) {
        uint8_t teamNamesSize{};
        std::wstring buf;
        ptr = Memory::copy(teamNamesSize, ptr);
        for(uint8_t i = 0; i < teamNamesSize; ++i) {
           ptr = Memory::copy(buf, ptr);
           teamNames_.push_back(buf);
        }
    }
    if(isViewerCountPresent()) {
        ptr = Memory::copy(viewerCount_, ptr);
    }

    //debug("DBG UDPServerDeltaSnapshot::buildFrom: %f", timeStamp_);

    while(payload + length > ptr + sizeof(tick_)) {
        MessageData msgData{};

        // 'K', 'Z', 'V', 'G', 'B', 'P', 'S', 'O', 'R'
        unsigned char gameObjectTypeId;
        ptr = Memory::copy(gameObjectTypeId, ptr);
        if(noneOf((char)gameObjectTypeId, 'K', 'P', 'S', 'O', 'R')) {
            debug("ERROR: UDPServerDeltaSnapshot::buildFrom: Illegal gameObjectTypeId");
        }

        msgData.type = gameObjectTypeId;
        msgData.field = UDPMessageData::Fields::Field_type;
        builtData_.push_back(msgData);

        uint32_t gameObjectId;
        ptr = Memory::copy(gameObjectId, ptr);
        msgData.id = gameObjectId;
        msgData.field = UDPMessageData::Fields::Field_id;
        builtData_.push_back(msgData);

        unsigned char attribListSize;
        ptr = Memory::copy(attribListSize, ptr);

        unsigned char fieldId;

        for(unsigned char i = 0; i < attribListSize; ++i) {
            ptr = Memory::copy(fieldId, ptr);
            if(fieldId > (unsigned char)UDPMessageData::Fields::Field_INDICATOR_REMOVED) {
                debug("ERROR: UDPServerDeltaSnapshot::buildFrom: Illegal fieldId (i=%d)", (int)i);
            }
            switch (static_cast<UDPMessageData::Fields>(fieldId)) {
            case UDPMessageData::Fields::Field_id: // gameobjectid
                ptr = Memory::copy(msgData.id, ptr);
                break;
            case UDPMessageData::Fields::Field_x:
                ptr = Memory::copy(msgData.x, ptr);
                break;
            case UDPMessageData::Fields::Field_contentBottom:
                ptr = Memory::copy(msgData.contentBottom, ptr);
                break;
            case UDPMessageData::Fields::Field_frameIndex:
                ptr = Memory::copy(msgData.frameIndex, ptr);
                break;
            case UDPMessageData::Fields::Field_skinMin:
                ptr = Memory::copy(msgData.skinMin, ptr);
                break;
            case UDPMessageData::Fields::Field_skinMax:
                ptr = Memory::copy(msgData.skinMax, ptr);
                break;
            case UDPMessageData::Fields::Field_state:
                ptr = Memory::copy(msgData.state, ptr);
                break;
            case UDPMessageData::Fields::Field_gameObjectType:
                ptr = Memory::copy(msgData.gameObjectType, ptr);
                break;
            case UDPMessageData::Fields::Field_hitPoints:
                ptr = Memory::copy(msgData.hitPoints, ptr);
                break;
            case UDPMessageData::Fields::Field_maxHitPoints:
                ptr = Memory::copy(msgData.maxHitPoints, ptr);
                break;
            case UDPMessageData::Fields::Field_linkedGameObjectId:
                ptr = Memory::copy(msgData.linkedGameObjectId, ptr);
                break;
            case UDPMessageData::Fields::Field_dead:
                ptr = Memory::copy(msgData.dead, ptr);
                break;
            case UDPMessageData::Fields::Field_type:
                ptr = Memory::copy(msgData.type, ptr);
                break;
            case UDPMessageData::Fields::Field_flipped:
                ptr = Memory::copy(msgData.flipped, ptr);
                break;
            case UDPMessageData::Fields::Field_color:
                ptr = Memory::copy(msgData.color, ptr);
                break;
            case UDPMessageData::Fields::Field_messageKey:
                ptr = Memory::copy(msgData.messageKey, ptr);
                break;
            case UDPMessageData::Fields::Field_visible:
                ptr = Memory::copy(msgData.visible, ptr);
                break;
            case UDPMessageData::Fields::Field_teamIndex:
                ptr = Memory::copy(msgData.teamIndex, ptr);
                break;
            case UDPMessageData::Fields::Field_coolDown:
                ptr = Memory::copy(msgData.coolDown, ptr);
                break;
            case UDPMessageData::Fields::Field_amount:
                ptr = Memory::copy(msgData.amount, ptr);
                break;
            case UDPMessageData::Field_scores:
                ptr = Memory::copy(msgData.scores, ptr);
                break;
            case UDPMessageData::Field_elapsedTimes:
                ptr = Memory::copy(msgData.elapsedTimes, ptr);
                break;
            case UDPMessageData::Field_teamCount:
                ptr = Memory::copy(msgData.teamCount, ptr);
                break;
            case UDPMessageData::Field_looserTeam:
                ptr = Memory::copy(msgData.looserTeam, ptr);
                break;
            case UDPMessageData::Field_showCountDown:
                ptr = Memory::copy(msgData.showCountDown, ptr);
                break;
            case UDPMessageData::Field_spareTime:
                ptr = Memory::copy(msgData.spareTime, ptr);
                break;
            case UDPMessageData::Field_messageString:
                ptr = Memory::copy(msgData.string, ptr);
                break;
            case UDPMessageData::Fields::Field_INDICATOR_REMOVED:
                // Nothing to do
                break;
            }
            msgData.field = static_cast<UDPMessageData::Fields>(fieldId);
            builtData_.push_back(msgData);
        }
    }

    ptr = Memory::copy(tick_, ptr);
    clientConnection_ = clientConnection;
    return true;
}

bool UDPServerDeltaSnapshot::updateClientId(uint32_t clientId) noexcept {
    if(clientIdOffset_ == Serialize::SERIALIZE_ERROR || !payload_.get()) return false;
    clientId_ = clientId;
    memcpy(payload_.get() + clientIdOffset_, &clientId_, sizeof(clientId_));
    return true;
}

const unsigned char* UDPServerDeltaSnapshot::payload() noexcept {
    if(payload_.get()) return payload_.get();

    if(!serialize_.size()) return nullptr;
    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();
}

template<>
void UDPServerDeltaSnapshot::serialize<>(Serialize& item, unsigned char type, uint32_t id, UDPMessageData::Fields fieldId, std::wstring x1, std::wstring x2) {
    unsigned char fieldId2 = fieldId;
    if(x1 != x2) {
        serialize(item, type, id);
        item.add(fieldId2);
        item.addString(x2);
        ++diffListSize_;
    }
}

