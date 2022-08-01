#include "SoundEffect.h"
#include "Defines.h"

namespace {
THREAD_LOCAL std::string name_;
THREAD_LOCAL std::string buff_;
}

const char* SoundEffect::getName() noexcept {
    buff_ = name_;
    name_ = "";
    return buff_.c_str();
}

void SoundEffect::play(Name name) noexcept {
    switch(name) {
    case Name::PLAY_SWORD: name_ = "PLAY_SWORD"; break;
    case Name::STOP_PLAYING_SWORD: name_ = "STOP_PLAY_SWORD"; break;
    case Name::PLAY_GOAL: name_ = "PLAY_GOAL"; break;
    case Name::STOP_PLAYING_GOAL: name_ = "STOP_PLAY_GOAL"; break;
    case Name::PLAY_RUN_ON_SAND: name_ = "PLAY_RUN_ON_SAND"; break;
    case Name::STOP_PLAYING_RUN_ON_SAND: name_ = "STOP_PLAY_RUN_ON_SAND"; break;
    case Name::PLAY_POP: name_ = "PLAY_POP"; break;
    case Name::STOP_PLAYING_POP: name_ = "STOP_PLAY_POP"; break;
    case Name::PLAY_DRUMS: name_ = "PLAY_DRUMS"; break;
    case Name::STOP_PLAYING_DRUMS: name_ = "STOP_PLAY_DRUMS"; break;
    case Name::PLAY_GAME_BALL_TAP: name_ = "PLAY_GAME_BALL_TAP"; break;
    case Name::STOP_PLAYING_GAME_BALL_TAP: name_ = "STOP_PLAY_GAME_BALL_TAP"; break;
    }
}

bool SoundEffect::isEmpty() noexcept {
    return !name_.length();
}
