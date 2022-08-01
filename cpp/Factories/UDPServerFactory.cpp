#include "UDPServerFactory.h"
#include "UDPServer.h"


UDPServerInterfaceUPtr UDPServerFactory::create(UDPMessageQueueInterface* q) noexcept {
    return std::unique_ptr<UDPServerInterface>(new UDPServer(q));
}
