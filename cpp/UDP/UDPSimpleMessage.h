#ifndef __UDPSIMPLEMESSAGE_H__
#define __UDPSIMPLEMESSAGE_H__

#include "UDP.h"
#include "Serialize.h"

class UDPSimpleMessage : public UDP::Message
{
    friend class UDPMessageFactory;
    UDPSimpleMessage(TypeId typeId) : typeId_(typeId) {}

public:
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return typeId_;}
    const unsigned char* payload() noexcept override;
    uint16_t buildNumber() const override {return buildNumber_;}

    bool exitMessage() const noexcept override {return typeId_ == TypeId::EXIT_GAME;}
    bool keepAlive() const noexcept override {return typeId_ == TypeId::KEEP_ALIVE;}


private:
    Serialize serialize_;
    TypeId typeId_;
    uint16_t buildNumber_{};
};

using UDPSimpleMessageUPtr = std::unique_ptr<UDPSimpleMessage>;

#endif // __UDSIMPLEMESSAGE_H__
