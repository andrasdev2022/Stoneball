#ifndef __UDPPACKAGECREATOR_H__
#define __UDPPACKAGECREATOR_H__

#include <inttypes.h>
#ifndef WIN32
    #include <arpa/inet.h>
#endif // #ifndef WIN32
#include <memory>
#include <unordered_map>
#include "UDPConnectionInterface.h"
#include <vector>
#include <map>
#include "Zlib_.h"

class UDPPackageCreator
{
public:
    struct Msg {
        uint8_t partCount;
        UDPConnectionInterfaceSPtr clientConnection;
        std::unique_ptr<uint8_t[]> data;
        uint16_t dataSize;
        std::vector<uint8_t> parts;
    };

    UDPPackageCreator(bool useZlib);
    int send(int64_t socketfd, const unsigned char* buff, const size_t size, int flag, const struct sockaddr* addr, socklen_t len);

    void assembleMessage(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept;

    size_t incompleteMessageCount() const noexcept {return incompleteMessages_.size();}
    size_t completeMessageCount() const noexcept {return completeMessages_.size();}

    const Msg& nextCompleteMessage() noexcept {return completeMessages_.begin()->second;}
    void popNextCompleteMessage() noexcept {completeMessages_.erase(completeMessages_.begin());}

private:
    bool initZlib() noexcept;
    std::unordered_map<uint32_t, Msg> incompleteMessages_;
    std::map<uint32_t, Msg> completeMessages_;
    bool useZlib_{};
    ZlibUPtr zlib_;
};

using UDPPackageCreatorUPrt = std::unique_ptr<UDPPackageCreator>;
using UDPPackageCreatorSPrt = std::shared_ptr<UDPPackageCreator>;

#endif // UDPPACKAGECREATOR_H
