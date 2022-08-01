#ifndef __SINGLEPLAYERGAME_H__
#define __SINGLEPLAYERGAME_H__

#include "Game.h"

class SingleplayerGame : public Game
{
public:
    SingleplayerGame(); // in order to avoid incomplete type errors
    void updateWorld(double now) noexcept override;
    void drawWorld(AndroidBitmapInfo* info, void* pixels) noexcept override;

    // dynamic_cast
    SingleplayerGame* singleplayerGame() noexcept override {return this;}
};

#endif // __SINGLEPLAYERGAME_H__
