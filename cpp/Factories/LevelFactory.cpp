#include "LevelFactory.h"
#ifndef __SERVERAPP__
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "Level8.h"
#include "Level9.h"
#include "Level10.h"
#include "Level11.h"
#include "Level12.h"
#include "Level13.h"
#include "Level14.h"
#include "Level15.h"
#include "Level16.h"
#include "Level17.h"
#include "Level18.h"
#include "Level19.h"
#include "Level20.h"
#include "Level21.h"
#include "Level22.h"
#include "Level23.h"
#include "Level24.h"
#include "Level25.h"
#include "Level26.h"
#include "Level27.h"
#include "Level28.h"
#include "Level29.h"
#include "Level30.h"
#include "Level31.h"
#include "Level32.h"
#endif
#include "PlayWithFriend1.h"
#include "Multiplayer2.h"
#include "PlayWithFriend2.h"
#include "PlayWithFriend3.h"
#include "PlayWithFriend4.h"
#include "PlayWithFriend5.h"
#include "PlayWithFriend6.h"
#include "Multiplayer3.h"
#include "Multiplayer4.h"
#include "Multiplayer5.h"
#include "Multiplayer6.h"

std::unique_ptr<Tileset> LevelFactory::create(GameLevel gameLevel) {
    TilesetUPtr tileset;
    //PositionI levelPosition(20_ZRW, 25_ZRH);
#ifndef __SERVERAPP__
    if(gameLevel == GameLevel::LEVEL_1) {
        tileset = std::make_unique<Level2>();
    } else if(gameLevel == GameLevel::LEVEL_2) {
        tileset = std::make_unique<Level4>();
    } else if(gameLevel == GameLevel::LEVEL_3) {
        tileset = std::make_unique<Level5>();
    } else if(gameLevel == GameLevel::LEVEL_4) {
        tileset = std::make_unique<Level6>();
    } else if(gameLevel == GameLevel::LEVEL_5) {
        tileset = std::make_unique<Level7>();
    } else if(gameLevel == GameLevel::LEVEL_6) {
        tileset = std::make_unique<Level8>();
    } else if(gameLevel == GameLevel::LEVEL_7) {
        tileset = std::make_unique<Level9>();
    } else if(gameLevel == GameLevel::LEVEL_8) {
        tileset = std::make_unique<Level10>();
    } else if(gameLevel == GameLevel::LEVEL_9) {
        tileset = std::make_unique<Level11>();
    } else if(gameLevel == GameLevel::LEVEL_10) {
        tileset = std::make_unique<Level12>();
    } else if(gameLevel == GameLevel::LEVEL_11) {
        tileset = std::make_unique<Level13>();
    } else if(gameLevel == GameLevel::LEVEL_12) {
        tileset = std::make_unique<Level14>();
    } else if(gameLevel == GameLevel::LEVEL_13) {
        tileset = std::make_unique<Level15>();
    } else if(gameLevel == GameLevel::LEVEL_14) {
        tileset = std::make_unique<Level16>();
    } else if(gameLevel == GameLevel::LEVEL_15) {
        tileset = std::make_unique<Level17>();
    } else if(gameLevel == GameLevel::LEVEL_16) {
        tileset = std::make_unique<Level18>();
    } else if(gameLevel == GameLevel::LEVEL_17) {
        tileset = std::make_unique<Level19>();
    } else if(gameLevel == GameLevel::LEVEL_18) {
        tileset = std::make_unique<Level20>();
    } else if(gameLevel == GameLevel::LEVEL_19) {
        tileset = std::make_unique<Level21>();
    } else if(gameLevel == GameLevel::LEVEL_20) {
        tileset = std::make_unique<Level22>();
    } else if(gameLevel == GameLevel::LEVEL_21) {
        tileset = std::make_unique<Level23>();
    } else if(gameLevel == GameLevel::LEVEL_22) {
        tileset = std::make_unique<Level24>();
    } else if(gameLevel == GameLevel::LEVEL_23) {
        tileset = std::make_unique<Level25>();
    } else if(gameLevel == GameLevel::LEVEL_24) {
        tileset = std::make_unique<Level26>();
    } else if(gameLevel == GameLevel::LEVEL_25) {
        tileset = std::make_unique<Level27>();
    } else if(gameLevel == GameLevel::LEVEL_26) {
        tileset = std::make_unique<Level28>();
    } else if(gameLevel == GameLevel::LEVEL_27) {
        tileset = std::make_unique<Level29>();
    } else if(gameLevel == GameLevel::LEVEL_28) {
        tileset = std::make_unique<Level30>();
    } else if(gameLevel == GameLevel::LEVEL_29) {
        tileset = std::make_unique<Level31>();
    } else if(gameLevel == GameLevel::LEVEL_30) {
        tileset = std::make_unique<Level32>();
    } else
#endif
    if(gameLevel == GameLevel::LEVEL_PLAY_WITH_FRIEND_1) {
        tileset = std::make_unique<PlayWithFriend1>();
    } else if(gameLevel == GameLevel::LEVEL_PLAY_WITH_FRIEND_2) {
        tileset = std::make_unique<PlayWithFriend2>();
    } else if(gameLevel == GameLevel::LEVEL_PLAY_WITH_FRIEND_3) {
        tileset = std::make_unique<PlayWithFriend3>();
    } else if(gameLevel == GameLevel::LEVEL_PLAY_WITH_FRIEND_4) {
        tileset = std::make_unique<PlayWithFriend4>();
    } else if(gameLevel == GameLevel::LEVEL_PLAY_WITH_FRIEND_5) {
        tileset = std::make_unique<PlayWithFriend5>();
    } else if(gameLevel == GameLevel::LEVEL_PLAY_WITH_FRIEND_6) {
        tileset = std::make_unique<PlayWithFriend6>();
    } else if(gameLevel == GameLevel::LEVEL_MULTIPLAYER_MAP1) {
        tileset = std::make_unique<Multiplayer1>();
    } else if(gameLevel == GameLevel::LEVEL_MULTIPLAYER_MAP2) {
        tileset = std::make_unique<Multiplayer2>();
    } else if(gameLevel == GameLevel::LEVEL_MULTIPLAYER_MAP3) {
        tileset = std::make_unique<Multiplayer3>();
    } else if(gameLevel == GameLevel::LEVEL_MULTIPLAYER_MAP4) {
        tileset = std::make_unique<Multiplayer4>();
    } else if(gameLevel == GameLevel::LEVEL_MULTIPLAYER_MAP5) {
        tileset = std::make_unique<Multiplayer5>();
    } else if(gameLevel == GameLevel::LEVEL_MULTIPLAYER_MAP6) {
        tileset = std::make_unique<Multiplayer6>();
    }
    return tileset;
}
