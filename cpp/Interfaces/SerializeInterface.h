#ifndef __SERIALIZEINTERFACE_H__
#define __SERIALIZEINTERFACE_H__

#include <inttypes.h>

class Serialize;
class UDPMessageData;

class SerializeInterface {
public:
    virtual ~SerializeInterface() = default;
    virtual bool addData(Serialize*) = 0;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    //virtual const uint8_t* buildFrom(UDPMessageData*, const uint8_t*) = 0;
};

#endif // __SERIALIZEINTERFACE_H__
