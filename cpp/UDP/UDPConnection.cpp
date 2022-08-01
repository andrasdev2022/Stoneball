#include "UDPConnection.h"

#include <string.h>
#ifndef WIN32
    #include <unistd.h>
    #include <arpa/inet.h>
#endif // #ifndef WIN32
#include <string>
#include "CommonUtil.h"
#include "Global.h"

bool UDPConnectionInterfaceCmp::operator()(const UDPConnectionInterfaceSPtr& c1, const UDPConnectionInterfaceSPtr& c2) const noexcept {
    // the address has a "Pad" part in in.h, it should not be compared
    //if(c1->len() != c2->len()) return c1->len() < c2->len();
    constexpr size_t size = sizeof(sockaddr_in::sin_addr) + sizeof(sockaddr_in::sin_port) + sizeof(sockaddr_in::sin_family);
    bool retv = memcmp(&c1->addr(), &c2->addr(),  size) < 0;
    return retv;
}

UDPConnection::UDPConnection(int64_t socketfd, struct sockaddr_in& addr, socklen_t len, const UDPPackageCreatorSPrt& packageCreator) noexcept
: socketfd_(socketfd)
, addr_(addr)
, len_(len)
, creationTimeStamp_(now_ms())
, updateTimeStamp_(creationTimeStamp_)
, packageCreator_(packageCreator)
{
    //debug("UDPConnection::UDPConnection: len: %u, addr.sin_family: %u, sin_port: %u, sin_addr.s_addr: %u",
    //      len, addr.sin_family, addr.sin_port, addr.sin_addr.s_addr);
}

UDPConnection::~UDPConnection() noexcept {
    if(socketfd_ > 0) {
        debug("UDPConnection::~UDPConnection, socket closed");
        close(socketfd_);
    }
}

std::string UDPConnection::sinAddrPort() const noexcept {
    return std::to_string(addr_.sin_addr.s_addr) + ":" + std::to_string(addr_.sin_port);
}

std::string UDPConnection::host() const noexcept {
    return std::to_string(addr_.sin_addr.s_addr);
}

uint16_t UDPConnection::port() const noexcept {
    return addr_.sin_port;
}

void UDPConnection::send(const unsigned char* buff, size_t size) const noexcept {
    //auto sent = sendto(socketfd_, buff, size, MSG_CONFIRM, (const struct sockaddr *)&addr_, len_);
    auto sent = packageCreator_->send(socketfd_, buff, size, MSG_CONFIRM, (const struct sockaddr *)&addr_, len_);
    if(sent == -1) {
        debug("ERROR UDPConnection::send sendto failed, errno: %d", errno);
    } else if(sent != (int)size) {
        debug("WARNING UDPConnection::send sendto did not send the full package: %d/%u", sent, size);
    }
}

bool UDPConnection::expired(double periondMillis) const noexcept {
    // the socket was created an hour ago or there has no communication for 2 * player turn + 30 sec
    //double now = now_ms();
    //return now - creationTimeStamp_ > 3'600'000. || now - updateTimeStamp_ > 2 * 60'000.;
    return now_ms() - updateTimeStamp_ > periondMillis;
}

void UDPConnection::updateTimeStamp() noexcept {
    updateTimeStamp_ = now_ms();
}
