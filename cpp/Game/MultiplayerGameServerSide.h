#ifndef __MULTIPLAYERGAMESERVERSIDE_H__
#define __MULTIPLAYERGAMESERVERSIDE_H__

#include "MultiplayerGame.h"

class MultiplayerGameServerSide : public MultiplayerGame {
public:
    MultiplayerGameServerSide(); // in order to avoid incomplete type errors
    void updateWorld(double now) noexcept override;
    void drawWorld(AndroidBitmapInfo* info, void* pixels) noexcept override;
};

#endif // __MULTIPLAYERGAMESERVERSIDE_H__
