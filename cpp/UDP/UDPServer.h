#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__

#include <thread>
#include <memory>
#ifndef WIN32
    #include <sys/socket.h>
#else
    #define SHUT_RDWR SD_BOTH
#endif // #ifndef WIN32
#include "UDPServerInterface.h"
#include "UDPMessageInterface.h"
#include <set>

class UDPMessageQueueInterface;

class UDPServer : public UDPServerInterface {
    UDPServer(UDPMessageQueueInterface* udpMessageQueue) noexcept;
    friend class UDPServerFactory;
public:
    UDPServer(const UDPServer&) = delete;
    UDPServer(UDPServer&&) = delete;
    UDPServer& operator=(const UDPServer&) = delete;
    UDPServer& operator=(UDPServer&&) = delete;

    void start(int port) noexcept override;
    void run() noexcept override;
    void stop() noexcept override {stopped_ = true;if(sockfd_) shutdown(sockfd_, SHUT_RDWR);}
    void join() noexcept override;
    bool isRunning() const noexcept override {return running_;}
    int64_t socketFd() const noexcept override {return sockfd_;}

    // TODO: refact
    bool readClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;

    bool removeClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;

private:
    bool running_{false};
    bool stopped_{false};
    std::thread thread_;
    UDPMessageQueueInterface* udpMessageQueue_;
    int64_t sockfd_{0};
    std::set<UDPConnectionInterfaceSPtr, UDPConnectionInterfaceCmp> clientConnections_;
    int port_{};
};

using UDPServerUPtr = std::unique_ptr<UDPServer>;
#endif // #ifndef __UDPSERVER_H__
