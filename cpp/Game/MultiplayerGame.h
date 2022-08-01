#ifndef __MULTIPLAYERGAME_H__
#define __MULTIPLAYERGAME_H__

#include "Game.h"

class MultiplayerGame : public Game
{
public:
    MultiplayerGame(); // in order to avoid incomplete type errors
    void updateWorld(double now) noexcept override;
    void drawWorld(AndroidBitmapInfo* info, void* pixels) noexcept override;

private:
    WorldUPtr entityInterpolation(double now) noexcept;
};

#endif // __MULTIPLAYERGAME_H__
