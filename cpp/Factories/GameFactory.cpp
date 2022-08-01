#include "GameFactory.h"
#include "SingleplayerGame.h"
#include "MultiplayerGame.h"


GameInterfaceUPtr GameFactory::create(Type type) {
    GameInterfaceUPtr retV;
    switch(type) {
    case Type::SINGLEPLAYER: retV = GameInterfaceUPtr(new SingleplayerGame());
        break;
    case Type::MULTIPLAYER: retV = GameInterfaceUPtr(new MultiplayerGame());
        break;
    }
    return retV;
}
