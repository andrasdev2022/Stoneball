#ifndef __UDPMESSAGEDATA_H__
#define __UDPMESSAGEDATA_H__

#include <inttypes.h>
#include <string>

class UDPMessageData {
public:
    uint32_t id{}; // gameobjectid
    double x{};
    double contentBottom{};
    uint16_t frameIndex{};
    uint16_t skinMin{};
    uint16_t skinMax{};
    unsigned char state{}; // idle, attack, run, etc.
    unsigned char gameObjectType{}; // e.g.: GameObject::Type::GOT_KNIGHT_BLUE
    int32_t hitPoints{};
    int32_t maxHitPoints{};
    int32_t linkedGameObjectId{}; // questionmark uses it
    unsigned char dead{}; // 0 = no, 1 = yes
    unsigned char type{}; // K, Z, V, G, B, P, S, O, R
    unsigned char flipped{}; // 0 = no, 1 = yes
    unsigned char color{}; // filled by rolling stone
    unsigned char messageKey{}; // e.g. from messagebox
    unsigned char visible{}; // filled by cads and questionmark, // 0 = no, 1 = yes
    uint8_t teamIndex{255}; // filled by cards, emoteFigure
    uint8_t amount{255}; // filled by cards
    double coolDown{}; // filled by cards
    uint32_t scores{}; // filled by MatchResults
    uint64_t elapsedTimes{}; // filled by MatchResults
    uint8_t teamCount{};  // filled by MatchResults
    uint8_t looserTeam{255};  // filled by MatchResults
    uint8_t showCountDown{};  // filled by Messagebox1
    double spareTime{};  // filled by Messagebox1
    std::wstring messageString; // filled by Messagebox1

    const uint8_t* buildFrom(const uint8_t* ptr) noexcept;

    enum Fields {
        Field_id,
        Field_x,
        Field_contentBottom,
        Field_frameIndex,
        Field_skinMin,
        Field_skinMax,
        Field_state,
        Field_gameObjectType,
        Field_hitPoints,
        Field_maxHitPoints,
        Field_linkedGameObjectId,
        Field_dead,
        Field_type,
        Field_flipped,
        Field_color,
        Field_messageKey,
        Field_visible,
        Field_teamIndex,
        Field_amount,
        Field_coolDown,
        Field_scores,
        Field_elapsedTimes,
        Field_teamCount,
        Field_looserTeam,
        Field_showCountDown,
        Field_spareTime,
        Field_messageString,
        Field_INDICATOR_REMOVED,
    };
};

#endif // __UDPMESSAGEDATA_H
