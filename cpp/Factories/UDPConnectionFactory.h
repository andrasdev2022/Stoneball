#ifndef __UDPCONNECTIONFACTORY_H__
#define __UDPCONNECTIONFACTORY_H__

#include "UDPConnectionInterface.h"

class UDPConnectionFactory
{
public:
    static UDPConnectionInterfaceUPtr create(int64_t socketfd, struct sockaddr_in& addr, socklen_t len) noexcept;
};

#endif // __UDPCONNECTIONFACTORY_H__
