#ifndef __UDPMESSAGEQUEUE_H__
#define __UDPMESSAGEQUEUE_H__

#include <stddef.h>
#ifndef WIN32
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
#endif // #ifndef WIN32
#include <memory>
#include <string.h>
#include <mutex>

#include "UDP.h"
#include "UDPConnectionInterface.h"
#include "UDPMessageQueueInterface.h"
#include "UDPPackageCreator.h"

class UDPMessageQueue : public UDPMessageQueueInterface {
public:
    struct Item;
    UDPMessageQueue() : packageCreator_(true) {}
    bool addMessage(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    UDPMessageInterfaceUPtr pop() noexcept override;
    size_t size() const noexcept override;

private:
    mutable std::mutex mutex_;
    UDPPackageCreator packageCreator_;
};

#endif // #ifndef __UDPMESSAGEQUEUE_H__
