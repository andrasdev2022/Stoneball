#include "GameFactory.h"
#include "SingleplayerGame.h"
#include "MultiplayerGameServerSide.h"
#include "CommonUtil.h"

GameInterfaceUPtr GameFactory::create(Type type) {
    GameInterfaceUPtr retV;
    switch(type) {
    case Type::SINGLEPLAYER: retV = GameInterfaceUPtr(new SingleplayerGame());
        debug("ERROR: GameFactoryServerSide GameFactory::create: SingleplayerGame");
        break;
    case Type::MULTIPLAYER: retV = GameInterfaceUPtr(new MultiplayerGameServerSide());
        break;
    }
    return retV;
}
