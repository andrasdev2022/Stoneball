#include "UDPPackageCreator.h"
#include "IdGenerator.h"
#include "Serialize.h"
#ifndef WIN32
    #include <arpa/inet.h>
#else
    #include <winsock.h>
#endif // #ifndef WIN32
#include "Memory.h"
#include <algorithm>
#include "CommonUtil.h"

#ifndef WIN32
    ssize_t (*sendtoTest)(int socketfd, const void* buff, size_t size, int flag, const struct sockaddr* addr, socklen_t len) = ::sendto;
#else
    int (__stdcall *sendtoTest)(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen) = ::sendto;
#endif

static constexpr uint32_t packageSize = 500;

UDPPackageCreator::UDPPackageCreator(bool useZlib) : useZlib_(useZlib) {
    debug("DEBUG UDPPackageCreator::UDPPackageCreator !!!!!!!!!!!!!!!!!!!!!!!!");
}

bool UDPPackageCreator::initZlib() noexcept {
    if(!useZlib_) return false;
    if(!zlib_) {
        zlib_ = std::make_unique<Zlib>();
    }
    return true;
}

int UDPPackageCreator::send(int64_t socketfd, const unsigned char* msgBuff, const size_t msgSize, int flag, const struct sockaddr* addr, socklen_t len) {
    const unsigned char* buff;
    size_t size;
    if(initZlib()) {
        if(!zlib_->deflateMsg(msgBuff, msgSize)) {
            debug("ERROR: UDPPackageCreator::send deflateMsg");
            // do not send unzipped package
            //buff = msgBuff;
            //size = msgSize;
            return -1;
        } else {
            buff = zlib_->deflateBuffer();
            size = zlib_->deflateOutSize();
        }
    } else {
        debug("ERROR: UDPPackageCreator::send initZlib");
        buff = msgBuff;
        size = msgSize;
    }

    const uint32_t id = IdGenerator::generateId();
    const uint8_t partCount = (size / packageSize) + ((size % packageSize) != 0 ? 1 : 0);
    size_t sizeCopy = size;

    const uint8_t* partData = buff;
    for(uint8_t partIndex = 0; partIndex < partCount; ++partIndex) {
        Serialize serialize;
        serialize.add(id);
        serialize.add(partCount);
        serialize.add(partIndex);
        uint16_t partSize;
        if(sizeCopy <= packageSize) {
            partSize = sizeCopy;
            sizeCopy = 0;
        } else {
            partSize = packageSize;
            sizeCopy -= packageSize;
        }
        serialize.add(partSize);
        serialize.add3(partData, partSize);
        auto package = serialize.get();
        if(partIndex == 1) {
            debug("Package sent with multiple parts");
        }
        //debug("DEBUG UDPPackageCreator::send id: %u, partcount: %u, partIndex: %u, partSize: %u, serialize.size(): %u, size: %u, msgSize: %u",
        //      id, partCount, partIndex, partSize, serialize.size(), size, msgSize);
        if(sendtoTest(socketfd, (const char*)package.get(), serialize.size(), flag, addr, len) == -1) {
            return -1;
        }
        partData += packageSize;
    }
    return msgSize;
}

void UDPPackageCreator::assembleMessage(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    initZlib();
    uint32_t id;
    auto ptr = Memory::copy(id, payload);

    uint8_t partCount;
    ptr = Memory::copy(partCount, ptr);

    uint8_t partIndex;
    ptr = Memory::copy(partIndex, ptr);

    uint16_t partSize;
    ptr = Memory::copy(partSize, ptr);

    if(length != (size_t)partSize + 8) {
        debug("ERROR: UDPPackageCreator::assembleMessage lenght problem: %u, %u", length, (size_t)partSize + 8);
        debug("DEBUG: id: %u, partcount: %u, partIndex: %u", id, partCount, partIndex);
        return;
    }

    if(auto it = incompleteMessages_.find(id);it != incompleteMessages_.end()) {
        if(it->second.partCount == partCount) {
            it->second.parts[partIndex] = 1;
            memcpy(it->second.data.get() + (uint32_t)partIndex * packageSize, (const char*)ptr, partSize);
            it->second.clientConnection = clientConnection;
            it->second.dataSize += partSize;
            size_t sum = std::count(it->second.parts.begin(), it->second.parts.end(), 1);
            if(sum == it->second.parts.size()) {
                // the message is complete, what to do?
                debug("Package received with multiple parts, partCount: %u", partCount);

                if(zlib_) {
                    if(!zlib_->inflateMsg(it->second.data.get(), it->second.dataSize)) {
                        debug("ERROR: UDPPackageCreator::assembleMessage inflateMsg 1");
                        // we do not handle non-zipped packages
                        incompleteMessages_.erase(it);
                        return;
                    } else {
                        it->second.data = std::make_unique<uint8_t[]>(zlib_->deflateOutSize());
                        memcpy(it->second.data.get(), zlib_->deflateBuffer(), zlib_->deflateOutSize());
                        it->second.dataSize = zlib_->deflateOutSize();
                    }
                }
                completeMessages_.emplace(id, std::move(it->second));
                incompleteMessages_.erase(it);
            }
        } else {
            debug("ERROR ?????????????????????????");
        }
    } else {
        Msg msg;
        msg.partCount = partCount;
        msg.data = std::make_unique<uint8_t[]>((uint32_t)partCount * packageSize);
        memcpy(msg.data.get() + (uint32_t)partIndex * packageSize, (const char*)ptr, partSize);
        msg.clientConnection = clientConnection;
        msg.parts = std::vector<uint8_t>(partCount, 0);
        msg.parts[partIndex] = 1;
        msg.dataSize = partSize;
        if(partCount != 1) {
            incompleteMessages_.emplace(id, std::move(msg));
        } else {
            if(zlib_) {
                if(!zlib_->inflateMsg(msg.data.get(), msg.dataSize)) {
                    //debug("ERROR: UDPPackageCreator::assembleMessage inflateMsg 2");
                    //debug("id: %u, partCount: %u, partIndex: %u, partSize: %u", id, partCount, partIndex, partSize);
                    // we do not handle non-zipped packages
                    return;
                } else {
                    msg.data = std::make_unique<uint8_t[]>(zlib_->deflateOutSize());
                    memcpy(msg.data.get(), zlib_->deflateBuffer(), zlib_->deflateOutSize());
                    msg.dataSize = zlib_->deflateOutSize();
                }
            }
            completeMessages_.emplace(id, std::move(msg));
        }
    }
}
