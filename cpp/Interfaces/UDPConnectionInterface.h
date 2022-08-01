#ifndef __UDPCONNECTIONINTERFACE_H__
#define __UDPCONNECTIONINTERFACE_H__

#include <memory>
#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
#else
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	//#include <winsock2.h>
	#include <ws2tcpip.h>
#endif
#include <string>

class UDPConnectionInterface;
using UDPConnectionInterfaceUPtr = std::unique_ptr<UDPConnectionInterface>;
using UDPConnectionInterfaceSPtr = std::shared_ptr<UDPConnectionInterface>;

class UDPConnectionInterface
{
public:
    virtual ~UDPConnectionInterface() = default;
    virtual int64_t socketfd() const noexcept = 0;
    virtual void resetSocketfd(int64_t s) noexcept = 0;
    virtual const struct sockaddr_in& addr() const noexcept = 0;
    virtual socklen_t len() const noexcept = 0;
    virtual std::string sinAddrPort() const noexcept = 0;
    virtual std::string host() const noexcept = 0;
    virtual uint16_t port() const noexcept = 0;

    virtual void send(const unsigned char* buff, size_t size) const noexcept = 0;
    virtual bool expired(double periondMillis) const noexcept = 0;
    virtual void updateTimeStamp() noexcept = 0;
};

struct UDPConnectionInterfaceCmp {
    bool operator()(const UDPConnectionInterfaceSPtr& c1, const UDPConnectionInterfaceSPtr& c2) const noexcept;
};

#endif // __UDPCONNECTIONINTERFACE_H__
