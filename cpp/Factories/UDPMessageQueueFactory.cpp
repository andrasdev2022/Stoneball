#include "UDPMessageQueueFactory.h"
#include "UDPMessageQueue.h"

UDPMessageQueueInterfaceUPtr UDPMessageQueueFactory::create() noexcept {
    return UDPMessageQueueInterfaceUPtr(new UDPMessageQueue());
}
