#ifndef __UDPSERVERFACTORY_H__
#define __UDPSERVERFACTORY_H__

#include "UDPServerInterface.h"
#include "UDPMessageQueueInterface.h"

class UDPServerFactory
{
public:
    static UDPServerInterfaceUPtr create(UDPMessageQueueInterface* q) noexcept;
};

#endif // __UDPSERVERFACTORY_H__
