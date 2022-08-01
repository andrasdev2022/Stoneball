#ifndef __SOUNDEFFECT_H__
#define __SOUNDEFFECT_H__

#include <string>

class SoundEffect final
{
public:
    enum class Name {
        PLAY_SWORD,
        STOP_PLAYING_SWORD,
        PLAY_GOAL,
        STOP_PLAYING_GOAL,
        PLAY_RUN_ON_SAND,
        STOP_PLAYING_RUN_ON_SAND,
        PLAY_POP,
        STOP_PLAYING_POP,
        PLAY_DRUMS,
        STOP_PLAYING_DRUMS,
        PLAY_GAME_BALL_TAP,
        STOP_PLAYING_GAME_BALL_TAP,
    };
    static const char* getName() noexcept;
    static void play(Name name) noexcept;
    static bool isEmpty() noexcept;
};

#endif // __SOUNDEFFECT_H__
