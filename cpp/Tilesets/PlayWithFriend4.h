#ifndef __PLAYWITHFRIEND4_H__
#define __PLAYWITHFRIEND4_H__

#include "PlayWithFriend3.h"

class PlayWithFriend4 : public PlayWithFriend3
{
public:
    PlayWithFriend4();
    void init(WorldUPtr& world) noexcept override;
};

#endif // __PLAYWITHFRIEND4_H__
