#include "UDPMessageQueue.h"
#include "EnumClassOp.h"
#include "UDPMessageFactory.h"
#include "CommonUtil.h"

struct UDPMessageQueue::Item {
    Item(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
        this->payload = std::make_unique<unsigned char[]>(length);
        this->length = length;
        this->clientConnection = clientConnection;
        memcpy(this->payload.get(), payload, length);
        //debug("DBG UDPMessageQueue::Item p: %p, length: %u", this->payload.get(), this->length);
    }
    std::unique_ptr<unsigned char[]> payload;
    size_t length;
    UDPConnectionInterfaceSPtr clientConnection;
};

bool UDPMessageQueue::addMessage(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    std::unique_lock<std::mutex> lck(mutex_);
    packageCreator_.assembleMessage(payload, length, clientConnection);
    return true;
}

UDPMessageInterfaceUPtr UDPMessageQueue::pop() noexcept {
    std::unique_lock<std::mutex> lck(mutex_);
    if(packageCreator_.completeMessageCount() == 0) return nullptr;
    const UDPPackageCreator::Msg& completeMessage = packageCreator_.nextCompleteMessage();
    Item item(completeMessage.data.get(), completeMessage.dataSize, completeMessage.clientConnection);
    packageCreator_.popNextCompleteMessage();
    lck.unlock();

    if(item.length > 0) {
        EnumClassOp op(UDPMessageInterface::TypeId::LAST_ONE);
        if(item.payload[0] >= (uint8_t)op ) {
            debug("ERROR: UDPMessageQueue::pop Illegal type id");
            return nullptr;
        }
        op = (int)item.payload[0];
        UDPMessageInterfaceUPtr msg = UDPMessageFactory::create(op);
        if(!msg) {
            if(op != UDPMessageInterface::TypeId::EXIT_GAME) {
                debug("ERROR: UDPMessageQueue::pop message creation failed");
            }
            return nullptr;
        }
        if(msg->buildFrom(item.payload.get(), item.length, item.clientConnection)) {
            //debug("UDP message received");
            return msg;
        }
        debug("ERROR: UDPMessageQueue::pop buildFrom failed");
    } else {
        debug("ERROR: UDPMessageQueue::pop item length is zero");
    }
    return nullptr;
}

size_t UDPMessageQueue::size() const noexcept {
    std::unique_lock<std::mutex> lck(mutex_);
    return packageCreator_.completeMessageCount();
}
