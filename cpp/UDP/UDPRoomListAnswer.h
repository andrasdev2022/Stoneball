#ifndef __UDPROOMLISTANSWER_H__
#define __UDPROOMLISTANSWER_H__

#include "UDP.h"
#include "Serialize.h"
#include "ButtonEventListener.h"
#include <list>

class UDPRoomListAnswer : public UDP::Message {
    friend class UDPMessageFactory;
    UDPRoomListAnswer() = default;
public:
    struct MessageData {
        int clientNum;
        std::wstring roomName;
        std::string host;
        int port;
        uint8_t selectedMapNr;
        uint8_t selectedMinutesNr;
        double registrationTime;
    };

    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::ROOM_LIST_ANSWER;}
    const unsigned char* payload() noexcept override;

    bool addData(MessageData data) noexcept;

    const std::list<MessageData>& roomList() const noexcept {return roomList_;}

    const std::wstring& lobbyCommInfo() const {return lobbyCommInfo_;}
    void setLobbyCommInfo(std::wstring lobbyCommInfo) {lobbyCommInfo_ = lobbyCommInfo;}

    UDPRoomListAnswer* roomListAnswer() noexcept override {return this;}
private:
    std::list<MessageData> roomList_;
    Serialize serialize_;
    std::wstring lobbyCommInfo_;
};

using UDPRoomListAnswerUPtr = std::unique_ptr<UDPRoomListAnswer>;

#endif // __UDPROOMLISTANSWER_H__
