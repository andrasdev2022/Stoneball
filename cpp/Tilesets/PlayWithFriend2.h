#ifndef __PLAYWITHFRIEND2_H__
#define __PLAYWITHFRIEND2_H__

#include "PlayWithFriend1.h"

class PlayWithFriend2 : public PlayWithFriend1
{
public:
    PlayWithFriend2();
    void init(WorldUPtr& world) noexcept override;

};

#endif // __PLAYWITHFRIEND2_H__
