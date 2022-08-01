#ifndef __GAMEOBJECTINTERFACE_H__
#define __GAMEOBJECTINTERFACE_H__

class BlueTriangle;
class Knight;

class GameObjectInterface {
public:
    enum class Type {
        GOT_KNIGHT_RED,
        GOT_KNIGHT_DARK,
        GOT_KNIGHT_BLONDE_FEMALE,
        GOT_KNIGHT_GREEN_FEMALE,
        GOT_CONTROLLERDISK1,
        GOT_CONTROLLERDISK2,
        GOT_ARROW_RIGHT,
        GOT_ARROW_UP,
        GOT_ARROW_LEFT,
        GOT_ARROW_DOWN,
        GOT_GREEN_GEM1,
        GOT_BLUE_GEM1,
        GOT_BULLET,
        GOT_PARTICLE,
        GOT_ROLLINGSTONE,
        GOT_RPGBGELEMENT,
        GOT_REFRESH,
        GOT_BACK,
        GOT_UNDO,
        GOT_NEXT_TURN,
        GOT_PASS,
        GOT_BLUE_TRIANGLE,
        GOT_RED_BOUNCING_STONE,
        GOT_GRAY_BOUNCING_STONE,
        GOT_YELLOW_BOUNCING_STONE,
        GOT_GREEN_BOUNCING_STONE,
        GOT_MESSAGEBOX,
        GOT_LISTBOX,
        GOT_SELECTIONBOX,
        GOT_DECK_OF_CARDS,
        GOT_CARD_PULL,
        GOT_CARD_PUSH_PLAYER,
        GOT_CARD_TELEPORT,
        GOT_QUESTIONMARK,
        GOT_SLIDER,
        GOT_MATCH_RESULTS,
        GOT_MESSAGEBOXYESNO,
        GOT_FIREWORK_LONG_BLUE,
        GOT_FIREWORK_LONG_ORANGE,
        GOT_FIREWORK_LONG_GREEN,
        GOT_TROPHY_NR1,
        GOT_TROPHY_NR2,
        GOT_TROPHY_NR3,
        GOT_EMOTE_BUTTON,
        GOT_EMOTES_BUBBLE,
        GOT_EMOTE_FIGURE,
        GOT_MESSAGEBOXTERMSOFUSE,
        COUNT
    };

    enum Event {
        ACTION_DOWN,
        ACTION_UP,
        ACTION_MOVE,
        ACTION_CANCEL,
        ACTION_OUTSIDE,
        ACTION_POINTER_DOWN,
        ACTION_POINTER_UP,
        ACTION_MASKED_POINTER_DOWN_1 = (ACTION_POINTER_DOWN | 0x0100),
        ACTION_MASKED_POINTER_UP_1   = (ACTION_POINTER_UP | 0x0100),
        ACTION_MASKED_POINTER_DOWN_2 = (ACTION_POINTER_DOWN | 0x0200),
        ACTION_MASKED_POINTER_UP_2   = (ACTION_POINTER_UP | 0x0200),
    };

    enum class Direction {
        NORTH,
        NORTHEAST,
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST,
        NORTHWEST
    };

    enum class Speed {
        ZERO,
        SLOW,
        FAST
    };

    virtual ~GameObjectInterface() = default;
    virtual BlueTriangle* blueTriangle() noexcept {return nullptr;}
    virtual Knight* knight() noexcept {return nullptr;}
};

#endif // __GAMEOBJECTINTERFACE_H__
