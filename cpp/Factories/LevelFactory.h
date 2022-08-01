#ifndef __LEVELFACTORY_H__
#define __LEVELFACTORY_H__

#include <memory>
#include <array>

class Tileset;

class LevelFactory {
public:
    // Stoneball.java must be synchronized in case of new level!!!!!!!!
    enum class GameLevel {
        LEVEL_1, LEVEL_2, LEVEL_3, LEVEL_4, LEVEL_5, LEVEL_6, LEVEL_7, LEVEL_8, LEVEL_9, LEVEL_10, LEVEL_11,
        LEVEL_12, LEVEL_13, LEVEL_14, LEVEL_15, LEVEL_16, LEVEL_17, LEVEL_18, LEVEL_19, LEVEL_20, LEVEL_21,
        LEVEL_22, LEVEL_23, LEVEL_24, LEVEL_25, LEVEL_26, LEVEL_27, LEVEL_28, LEVEL_29, LEVEL_30,
        // ---------------------
        LEVEL_PLAY_WITH_FRIEND_1,
        LEVEL_PLAY_WITH_FRIEND_2,
        LEVEL_PLAY_WITH_FRIEND_3,
        LEVEL_PLAY_WITH_FRIEND_4,
        LEVEL_PLAY_WITH_FRIEND_5,
        LEVEL_PLAY_WITH_FRIEND_6,
        // -------------------------
        // Multiplayer levels
        LEVEL_MULTIPLAYER_MAP1,
        LEVEL_MULTIPLAYER_MAP2,
        LEVEL_MULTIPLAYER_MAP3,
        LEVEL_MULTIPLAYER_MAP4,
        LEVEL_MULTIPLAYER_MAP5,
        LEVEL_MULTIPLAYER_MAP6,
        LAST_ONE
    };

    static std::unique_ptr<Tileset> create(GameLevel);
    static constexpr int holesCount = static_cast<int>(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1);
    // Levels:                                                     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
    static constexpr std::array<int, holesCount> minimumPushCount {36, 20, 18, 10, 15, 25, 37, 43, 41, 32, 69, 43, 14, 13, 21,
                                                            // 16  17  18  19  20   21  22   23   24   25  26  27  28  29  30
                                                               27, 35, 31, 99, 113, 71, 98, 121, 112, 102, 94, 84, 91, 86, 97};
};

#endif // __LEVELFACTORY_H__
