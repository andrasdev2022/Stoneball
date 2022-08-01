#ifndef __MULTIPLAYER2_H__
#define __MULTIPLAYER2_H__

#include "Multiplayer1.h"

class Multiplayer2 : public Multiplayer1
{
public:
    Multiplayer2();
    void init(WorldUPtr& world) noexcept override;

};

#endif // __MULTIPLAYER2_H__
