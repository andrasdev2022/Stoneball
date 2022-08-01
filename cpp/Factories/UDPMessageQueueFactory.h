#ifndef __UDPMESSAGEQUEUEFACTORY_H__
#define __UDPMESSAGEQUEUEFACTORY_H__

#include "UDPMessageQueueInterface.h"

class UDPMessageQueueFactory
{
public:
    static UDPMessageQueueInterfaceUPtr create() noexcept;
};

#endif // __UDPMESSAGEQUEUEFACTORY_H__
