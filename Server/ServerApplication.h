#ifndef __SERVERAPPLICATION_H__
#define __SERVERAPPLICATION_H__

/*
parameters:
 -timestep 15
 -maxplayercount n
 -matchingtimeout

*/

#include <string>
class UDPMessageQueue;

namespace ServerApplication {
    void InitServer(int port, int mapIndex, int minutes);
    void RunServer();
    size_t clientCount(); // players + viewers
    size_t playerCount(); // players
    void setFurtherPlayersViewOnly(bool v) noexcept;

    UDPMessageQueue* udpMessageQueue();
}

#endif // #ifndef __SERVERAPPLICATION_H__
