#ifndef __UDPCONNECTION_H__
#define __UDPCONNECTION_H__

#include "UDPConnectionInterface.h"
#include "UDPPackageCreator.h"

class UDPConnection : public UDPConnectionInterface
{
    UDPConnection(int64_t socketfd, struct sockaddr_in& addr, socklen_t len, const UDPPackageCreatorSPrt& packageCreator) noexcept;
    ~UDPConnection() noexcept;
    friend class UDPConnectionFactory;
public:
    int64_t socketfd() const noexcept override {return socketfd_;}
    void resetSocketfd(int64_t s) noexcept override {socketfd_ = s;}
    const struct sockaddr_in& addr() const noexcept override {return addr_;}
    socklen_t len() const noexcept override {return len_;}

    std::string sinAddrPort() const noexcept override;
    std::string host() const noexcept override;
    uint16_t port() const noexcept override;

    void send(const unsigned char* buff, size_t size) const noexcept override;
    bool expired(double periondMillis) const noexcept override;
    void updateTimeStamp() noexcept override;

private:
    int64_t socketfd_;
    struct sockaddr_in addr_;
    socklen_t len_;
    const double creationTimeStamp_;
    mutable double updateTimeStamp_;
    UDPPackageCreatorSPrt packageCreator_;
};

#endif // __UDPCONNECTION_H__
