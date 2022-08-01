#ifndef __UDPMESSAGEQUEUEINTERFACE_H__
#define __UDPMESSAGEQUEUEINTERFACE_H__

#include "UDPConnectionInterface.h"
#include "UDPMessageInterface.h"

class UDPMessageQueueInterface {
public:
    virtual ~UDPMessageQueueInterface() = default;
    virtual bool addMessage(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept = 0;
    virtual UDPMessageInterfaceUPtr pop() noexcept = 0;
    virtual size_t size() const noexcept = 0;
};

using UDPMessageQueueInterfaceUPtr = std::unique_ptr<UDPMessageQueueInterface>;

#endif // __UDPMESSAGEQUEUEINTERFACE_H__
