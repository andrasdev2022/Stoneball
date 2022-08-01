#ifndef __MULTIPLAYER5_H__
#define __MULTIPLAYER5_H__

#include "PlayWithFriend5.h"
#include "MultiplayerCommon.h"

class Multiplayer5 : public MultiplayerCommon<PlayWithFriend5>
{
public:
    Multiplayer5();

    Multiplayer5* multiplayer5() noexcept override {return this;}

private:
};

#endif // __MULTIPLAYER5_H__
