#ifndef __UDPSERVERINTERFACE_H__
#define __UDPSERVERINTERFACE_H__

#include "UDPConnectionInterface.h"

class UDPServerInterface {
public:
    virtual ~UDPServerInterface() = default;
    virtual void start(int port) noexcept = 0;
    virtual void run() noexcept = 0;
    virtual void stop() noexcept = 0;
    virtual void join() noexcept = 0;
    virtual bool isRunning() const noexcept  = 0;
    virtual int64_t socketFd() const noexcept  = 0;
    virtual bool readClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) noexcept = 0;
    virtual bool removeClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) noexcept = 0;

};

using UDPServerInterfaceUPtr = std::unique_ptr<UDPServerInterface>;

#endif // __UDPSERVERINTERFACE_H__
