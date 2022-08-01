#ifndef __GAMEFACTORY_H__
#define __GAMEFACTORY_H__

#include <memory>

class GameInterface;

class GameFactory
{
public:
    enum class Type {
        SINGLEPLAYER,
        MULTIPLAYER,
    };

    static std::unique_ptr<GameInterface> create(Type);
};

#endif // __GAMEFACTORY_H__
