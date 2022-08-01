#ifndef __MULTIPLAYER4_H__
#define __MULTIPLAYER4_H__

#include "Multiplayer3.h"

class Multiplayer4 : public Multiplayer3
{
public:
    Multiplayer4();
    void init(WorldUPtr& world) noexcept override;

};

#endif // __MULTIPLAYER4_H__
