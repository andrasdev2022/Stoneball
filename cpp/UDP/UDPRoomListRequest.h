#ifndef __UDPROOMLISTREQUEST_H__
#define __UDPROOMLISTREQUEST_H__

#include "UDP.h"
#include "Serialize.h"

class UDPRoomListRequest : public UDP::Message
{
    friend class UDPMessageFactory;
    UDPRoomListRequest(std::wstring messageData) : messageData_(messageData) {}

public:
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::ROOM_LIST_REQUEST;}
    const unsigned char* payload() noexcept override;
    uint16_t buildNumber() const override {return buildNumber_;}

    UDPRoomListRequest* roomListRequest() noexcept override {return this;}

    const std::wstring& messageData() const noexcept {return messageData_;}

private:
    Serialize serialize_;
    uint16_t buildNumber_{};
    std::wstring messageData_;
};

using UDPRoomListRequestUPtr = std::unique_ptr<UDPRoomListRequest>;

#endif // __UDPROOMLISTREQUEST_H__
