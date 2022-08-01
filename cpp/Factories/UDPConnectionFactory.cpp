#include "UDPConnectionFactory.h"
#include "UDPConnection.h"
#include "UDPPackageCreator.h"

namespace {
    thread_local UDPPackageCreatorSPrt packageCreator;

}

UDPConnectionInterfaceUPtr UDPConnectionFactory::create(int64_t socketfd, struct sockaddr_in& addr, socklen_t len) noexcept {
    if(!packageCreator) {
        packageCreator = std::make_shared<UDPPackageCreator>(true);
    }
    return std::unique_ptr<UDPConnectionInterface>(new UDPConnection(socketfd, addr, len, packageCreator));
}

