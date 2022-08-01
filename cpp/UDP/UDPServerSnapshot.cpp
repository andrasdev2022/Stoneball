#include "UDPServerSnapshot.h"
#include "GameObject.h"
#include "MultiplayerWorld.h"
#include "Rect.h"
#include "MediaData.h"
#include "Memory.h"

namespace {

const unsigned char* buldMessageDataFromPtr(const unsigned char* ptr, std::list<UDPMessageData>& messageDataList) {
    uint16_t size;
    ptr = Memory::copy(size, ptr);
    for(uint16_t i = 0; i < size; ++i) {
        UDPMessageData data;
        ptr = data.buildFrom(ptr);
        messageDataList.push_back(data);
    }
    return ptr;
}

} // namespace


UDPServerSnapshot::UDPServerSnapshot() noexcept {
   uint16_t num = 1;
    // Always Little Endian is sent
    if(*(char *)&num == 1) {
        //LOGI("Little-Endian");
        //debug("Little-Endian");
        // Little-Endian
        //ssize_t len = ::write(socket(), &val, sizeof(val));
        //if(len  != sizeof(val)) {
        //    throw Socket_Send_Error();
        //}
    } else {
        // Big-Endian
        //LOGI("Big-Endian");
        //debug("Big-Endian");
        //for(int i = sizeof(val) - 1; i >= 0; --i) {
        //    if(::write(socket(), ((char*)&val) + i, 1)  != 1) {
        //        throw Socket_Send_Error();
        //    }
        //}
    }
}
/*
void handle_eptr(std::exception_ptr eptr) // passing by value is ok
{
    try {
        if (eptr) {
            std::rethrow_exception(eptr);
        }
    } catch(const std::exception& e) {
        debug("DBG UDPServerSnapshot::buildFrom exception: '%s'", e.what());
    }
}
*/
bool UDPServerSnapshot::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        debug("ERROR: UDPServerSnapshot::buildFrom payload is null");
    }
    if(!length) {
        debug("ERROR: UDPServerSnapshot::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        debug("ERROR: UDPServerSnapshot::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;

    const unsigned char* ptr = payload + 1;

    ptr = Memory::copy(clientId_, ptr);
    ptr = Memory::copy(timeStamp_, ptr);

    //debug("DBG UDPServerSnapshot::buildFrom: %f", timeStamp_);

    ptr = buldMessageDataFromPtr(ptr, knights_);
    ptr = buldMessageDataFromPtr(ptr, particles_);
    ptr = buldMessageDataFromPtr(ptr, rollingStones_);
    ptr = buldMessageDataFromPtr(ptr, otherElements_);
    ptr = buldMessageDataFromPtr(ptr, rpgBgElements_);

    ptr = Memory::copy(moveHistorySize_, ptr);
    ptr = Memory::copy(activeKnightId_, ptr);
    ptr = Memory::copy(activeTeamId_, ptr);
    ptr = Memory::copy(score_, ptr);
    ptr = Memory::copy(elapsedGameTime1_, ptr);
    ptr = Memory::copy(elapsedGameTime2_, ptr);
    ptr = Memory::copy(elapsedGameTime3_, ptr);
    ptr = Memory::copy(elapsedGameTime4_, ptr);
    ptr = Memory::copy(playTime_, ptr);
    ptr = Memory::copy(winnerTeam_, ptr);
    ptr = Memory::copy(viewOnly_, ptr);
    ptr = Memory::copy(avatar_, ptr);

    uint8_t quittersSize{};
    uint8_t item{};
    ptr = Memory::copy(quittersSize, ptr);
    for(uint8_t i = 0; i < quittersSize; ++i) {
       ptr = Memory::copy(item, ptr);
       quitters_.push_back(item);
    }

    uint8_t teamNamesSize{};
    ptr = Memory::copy(teamNamesSize, ptr);
    std::wstring buff;
    for(uint8_t i = 0; i < teamNamesSize; ++i) {
       ptr = Memory::copy(buff, ptr);
       teamNames_.push_back(buff);
    }
    ptr = Memory::copy(viewerCount_, ptr);

    ptr = Memory::copy(tick_, ptr);
    clientConnection_ = clientConnection;

    //std::cout << "UDPServerSnapshot::buildFrom Knights: " << knights_.size() << "\n";
    return true;
}

bool UDPServerSnapshot::addData(const MultiplayerWorld* world, uint32_t clientId, double now, uint16_t moveHistorySize, uint32_t activeKnightId,
                                unsigned char activeTeamId, uint32_t score, uint16_t elapsedGameTime1, uint16_t elapsedGameTime2, uint16_t elapsedGameTime3,
                                uint16_t elapsedGameTime4, uint8_t playTime, uint8_t winnerTeam, uint8_t viewOnly, uint8_t avatar,
                                const std::vector<uint8_t>& quitters, std::vector<std::wstring>&& teamNames, uint16_t viewerCount) noexcept {
    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);

    clientIdOffset_ = serialize_.add(clientId);
    serialize_.add(now);

    serializeEntityMap(world->knightList());
    serializeEntityMap(world->particleList());
    serializeEntityMap(world->rollingStoneList());
    serializeEntityMap(world->otherElementList());
    serializeEntityMap(world->rpgBgElementList());

    serialize_.add(moveHistorySize);
    serialize_.add(activeKnightId);
    serialize_.add(activeTeamId);
    serialize_.add(score);
    serialize_.add(elapsedGameTime1);
    serialize_.add(elapsedGameTime2);
    serialize_.add(elapsedGameTime3);
    serialize_.add(elapsedGameTime4);
    serialize_.add(playTime);
    serialize_.add(winnerTeam);
    viewOnlyOffset_ = serialize_.add(viewOnly);
    avatarOffset_ = serialize_.add(avatar);
    serialize_.add((uint8_t)quitters.size());
    for(auto type : quitters) {
        serialize_.add(type);
    }
    serialize_.add((uint8_t)teamNames.size());
    for(const auto& name : teamNames) {
        serialize_.addString(name);
    }
    serialize_.add(viewerCount);

    return serialize_.add(++tick_) != Serialize::SERIALIZE_ERROR;
}

bool UDPServerSnapshot::updateClientId(uint32_t clientId) noexcept {
    if(clientIdOffset_ == Serialize::SERIALIZE_ERROR || !payload_.get()) return false;
    clientId_ = clientId;
    memcpy(payload_.get() + clientIdOffset_, &clientId_, sizeof(clientId_));
    return true;
}

bool UDPServerSnapshot::updateViewOnly(uint8_t viewOnly) noexcept {
    if(viewOnlyOffset_ == Serialize::SERIALIZE_ERROR || !payload_.get()) return false;
    viewOnly_ = viewOnly;
    memcpy(payload_.get() + viewOnlyOffset_, &viewOnly_, sizeof(viewOnly_));
    return true;
}

bool UDPServerSnapshot::updateAvatar(uint8_t avatar) noexcept {
    if(avatarOffset_ == Serialize::SERIALIZE_ERROR || !payload_.get()) return false;
    avatar_ = avatar;
    memcpy(payload_.get() + avatarOffset_, &avatar_, sizeof(avatar_));
    return true;
}

const unsigned char* UDPServerSnapshot::payload() noexcept {
    if(payload_.get()) return payload_.get();

    if(!serialize_.size()) return nullptr;
    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();
}
