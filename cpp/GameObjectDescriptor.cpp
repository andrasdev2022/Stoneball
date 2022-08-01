#include "GameObjectDescriptor.h"
#include "stoneball.h"
#include "MediaData.h"
#include "BitmapIds.h"

#include <array>

namespace {
    std::array<std::array<GameObjectDescriptor, static_cast<size_t>(State::Type::COUNT)>,
                    static_cast<size_t>(GameObjectInterface::Type::COUNT)> descriptor;
    bool descriptorInitialized = false;
}

const GameObjectDescriptor& GameObjectDescriptor::find(GameObjectInterface::Type got, State::Type st) noexcept {
    if(!descriptorInitialized) {
        initDescriptor();
    }
    return descriptor[static_cast<size_t>(got)][static_cast<size_t>(st)];
}

void GameObjectDescriptor::initDescriptor() noexcept {
    initButtonFrames();
    initRedKnightFrames();
    initDarkKnightFrames();
    initBlondeFemaleKnightFrames();
    initGreenFemaleKnightFrames();
    initEnvironmentFrames();
    initTriangle();
    initBouncingStones();
    initMessageboxes();
/*
    initBlueKnightFrames();
    initGreenKnightFrames();
    initWhiteKnightFrames();
    initYellowKnightFrames();
    initParticleFrames();
*/
}

void GameObjectDescriptor::initButtonFrames() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_ARROW_RIGHT)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_ARROW_RIGHT;
    d->skinMin_ = NATIVE_BITMAP_ARROW_RIGHT;
    d->skinMax_ = NATIVE_BITMAP_ARROW_RIGHT;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_ARROW_UP)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_ARROW_UP;
    d->skinMin_ = NATIVE_BITMAP_ARROW_UP;
    d->skinMax_ = NATIVE_BITMAP_ARROW_UP;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_ARROW_DOWN)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_ARROW_DOWN;
    d->skinMin_ = NATIVE_BITMAP_ARROW_DOWN;
    d->skinMax_ = NATIVE_BITMAP_ARROW_DOWN;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_ARROW_LEFT)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_ARROW_RIGHT; // it will be flipped
    d->skinMin_ = NATIVE_BITMAP_ARROW_RIGHT;
    d->skinMax_ = NATIVE_BITMAP_ARROW_RIGHT;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_REFRESH)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_REFRESH;
    d->skinMin_ = NATIVE_BITMAP_REFRESH;
    d->skinMax_ = NATIVE_BITMAP_REFRESH;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_BACK)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_ARROW_BACK;
    d->skinMin_ = NATIVE_BITMAP_ARROW_BACK;
    d->skinMax_ = NATIVE_BITMAP_ARROW_BACK;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_UNDO)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_UNDO;
    d->skinMin_ = NATIVE_BITMAP_UNDO;
    d->skinMax_ = NATIVE_BITMAP_UNDO;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_NEXT_TURN)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_NEXT_TURN;
    d->skinMin_ = NATIVE_BITMAP_NEXT_TURN;
    d->skinMax_ = NATIVE_BITMAP_NEXT_TURN;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_PASS)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_PASS;
    d->skinMin_ = NATIVE_BITMAP_PASS;
    d->skinMax_ = NATIVE_BITMAP_PASS;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_EMOTE_BUTTON)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_EMOTE_ICON;
    d->skinMin_ = NATIVE_BITMAP_EMOTE_ICON;
    d->skinMax_ = NATIVE_BITMAP_EMOTE_ICON;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_EMOTES_BUBBLE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_EMOTES_BUBBLE;
    d->skinMin_ = NATIVE_BITMAP_EMOTES_BUBBLE;
    d->skinMax_ = NATIVE_BITMAP_EMOTES_BUBBLE;
    d->animTimeStep_ = 10000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_EMOTE_FIGURE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_EMOTE_200IQ;
    d->skinMin_ = NATIVE_BITMAP_EMOTE_200IQ;
    d->skinMax_ = NATIVE_BITMAP_EMOTE_200IQ;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initRedKnightFrames() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_RED)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_2;
    d->skinMin_ = NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_2;
    d->skinMax_ = NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_2;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_RED)][static_cast<size_t>(State::Type::ST_RUN)];
    d->skinStart_ = NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_1;
    d->skinMin_ = NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_1;
    d->skinMax_ = NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_3;
    d->animTimeStep_ = 1000./8.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 320._ZRD / 4000.;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_RED)][static_cast<size_t>(State::Type::ST_SIT)];
    d->skinStart_ = NATIVE_BITMAP_MALE_RED_KNIGHT_SIT_1;
    d->skinMin_ = NATIVE_BITMAP_MALE_RED_KNIGHT_SIT_1;
    d->skinMax_ = NATIVE_BITMAP_MALE_RED_KNIGHT_SIT_1;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initDarkKnightFrames() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_DARK)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_2;
    d->skinMin_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_2;
    d->skinMax_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_2;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_DARK)][static_cast<size_t>(State::Type::ST_RUN)];
    d->skinStart_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_1;
    d->skinMin_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_1;
    d->skinMax_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_3;
    d->animTimeStep_ = 1000./8.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 320._ZRD / 4000.;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_DARK)][static_cast<size_t>(State::Type::ST_SIT)];
    d->skinStart_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_SIT_1;
    d->skinMin_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_SIT_1;
    d->skinMax_ = NATIVE_BITMAP_MALE_DARK_KNIGHT_SIT_1;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initBlondeFemaleKnightFrames() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_2;
    d->skinMin_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_2;
    d->skinMax_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_2;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE)][static_cast<size_t>(State::Type::ST_RUN)];
    d->skinStart_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_1;
    d->skinMin_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_1;
    d->skinMax_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_3;
    d->animTimeStep_ = 1000./8.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 320._ZRD / 4000.;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE)][static_cast<size_t>(State::Type::ST_SIT)];
    d->skinStart_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_SIT_1;
    d->skinMin_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_SIT_1;
    d->skinMax_ = NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_SIT_1;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initGreenFemaleKnightFrames() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_2;
    d->skinMin_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_2;
    d->skinMax_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_2;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE)][static_cast<size_t>(State::Type::ST_RUN)];
    d->skinStart_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_1;
    d->skinMin_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_1;
    d->skinMax_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_3;
    d->animTimeStep_ = 1000./8.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 320._ZRD / 4000.;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE)][static_cast<size_t>(State::Type::ST_SIT)];
    d->skinStart_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_SIT_1;
    d->skinMin_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_SIT_1;
    d->skinMax_ = NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_SIT_1;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initEnvironmentFrames() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_ROLLINGSTONE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_STONEBALL_01;
    d->skinMin_ = NATIVE_BITMAP_STONEBALL_01;
    d->skinMax_ = NATIVE_BITMAP_STONEBALL_01;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_RPGBGELEMENT)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_STONE_01;
    d->skinMin_ = NATIVE_BITMAP_STONE_01;
    d->skinMax_ = NATIVE_BITMAP_STONE_01;
    d->animTimeStep_ = 1000000.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initTriangle() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_BLUE_TRIANGLE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_BLUE_TRIANGLE_1;
    d->skinMin_ = NATIVE_BITMAP_BLUE_TRIANGLE_1;
    d->skinMax_ = NATIVE_BITMAP_BLUE_TRIANGLE_4;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_DECK_OF_CARDS)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_DECK_OF_CARDS;
    d->skinMin_ = NATIVE_BITMAP_DECK_OF_CARDS;
    d->skinMax_ = NATIVE_BITMAP_DECK_OF_CARDS;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_CARD_PULL)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_CARD_PULL;
    d->skinMin_ = NATIVE_BITMAP_CARD_PULL;
    d->skinMax_ = NATIVE_BITMAP_CARD_PULL;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_CARD_PUSH_PLAYER)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_CARD_PUSH_PLAYER;
    d->skinMin_ = NATIVE_BITMAP_CARD_PUSH_PLAYER;
    d->skinMax_ = NATIVE_BITMAP_CARD_PUSH_PLAYER;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_CARD_TELEPORT)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_CARD_TELEPORT;
    d->skinMin_ = NATIVE_BITMAP_CARD_TELEPORT;
    d->skinMax_ = NATIVE_BITMAP_CARD_TELEPORT;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_SELECTIONBOX)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_CARD_TELEPORT; // doesn't matter
    d->skinMin_ = NATIVE_BITMAP_CARD_TELEPORT; // doesn't matter
    d->skinMax_ = NATIVE_BITMAP_CARD_TELEPORT; // doesn't matter
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_QUESTIONMARK)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_QUESTIONMARK;
    d->skinMin_ = NATIVE_BITMAP_QUESTIONMARK;
    d->skinMax_ = NATIVE_BITMAP_QUESTIONMARK;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_SLIDER)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_CARD_TELEPORT; // doesn't matter
    d->skinMin_ = NATIVE_BITMAP_CARD_TELEPORT; // doesn't matter
    d->skinMax_ = NATIVE_BITMAP_CARD_TELEPORT; // doesn't matter
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_FIREWORK_LONG_BLUE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_EXPLOSION_LONG_BLUE_BEGIN;
    d->skinMin_ = NATIVE_BITMAP_EXPLOSION_LONG_BLUE_BEGIN;
    d->skinMax_ = NATIVE_BITMAP_EXPLOSION_LONG_BLUE_END;
    d->animTimeStep_ = 1000./17.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_FIREWORK_LONG_ORANGE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_EXPLOSION_LONG_ORANGE_BEGIN;
    d->skinMin_ = NATIVE_BITMAP_EXPLOSION_LONG_ORANGE_BEGIN;
    d->skinMax_ = NATIVE_BITMAP_EXPLOSION_LONG_ORANGE_END;
    d->animTimeStep_ = 1000./17.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_FIREWORK_LONG_GREEN)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_EXPLOSION_LONG_GREEN_BEGIN;
    d->skinMin_ = NATIVE_BITMAP_EXPLOSION_LONG_GREEN_BEGIN;
    d->skinMax_ = NATIVE_BITMAP_EXPLOSION_LONG_GREEN_END;
    d->animTimeStep_ = 1000./17.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_TROPHY_NR1)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_TROPHY_NR1_BEGIN;
    d->skinMin_ = NATIVE_BITMAP_TROPHY_NR1_BEGIN;
    d->skinMax_ = NATIVE_BITMAP_TROPHY_NR1_END;
    d->animTimeStep_ = 1000./10.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_TROPHY_NR2)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_TROPHY_NR2_BEGIN;
    d->skinMin_ = NATIVE_BITMAP_TROPHY_NR2_BEGIN;
    d->skinMax_ = NATIVE_BITMAP_TROPHY_NR2_END;
    d->animTimeStep_ = 1000./10.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_TROPHY_NR3)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_TROPHY_NR3_BEGIN;
    d->skinMin_ = NATIVE_BITMAP_TROPHY_NR3_BEGIN;
    d->skinMax_ = NATIVE_BITMAP_TROPHY_NR3_END;
    d->animTimeStep_ = 1000./10.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initBouncingStones() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_RED_BOUNCING_STONE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_RED_BOUNCING_STONE_1;
    d->skinMin_ = NATIVE_BITMAP_RED_BOUNCING_STONE_1;
    d->skinMax_ = NATIVE_BITMAP_RED_BOUNCING_STONE_4;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_GRAY_BOUNCING_STONE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_GRAY_BOUNCING_STONE_1;
    d->skinMin_ = NATIVE_BITMAP_GRAY_BOUNCING_STONE_1;
    d->skinMax_ = NATIVE_BITMAP_GRAY_BOUNCING_STONE_4;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_YELLOW_BOUNCING_STONE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_YELLOW_BOUNCING_STONE_1;
    d->skinMin_ = NATIVE_BITMAP_YELLOW_BOUNCING_STONE_1;
    d->skinMax_ = NATIVE_BITMAP_YELLOW_BOUNCING_STONE_4;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_GREEN_BOUNCING_STONE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_GREEN_BOUNCING_STONE_1;
    d->skinMin_ = NATIVE_BITMAP_GREEN_BOUNCING_STONE_1;
    d->skinMax_ = NATIVE_BITMAP_GREEN_BOUNCING_STONE_4;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}

void GameObjectDescriptor::initMessageboxes() noexcept {
    auto* d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_MESSAGEBOX)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_MESSAGEBOX1;
    d->skinMin_ = NATIVE_BITMAP_MESSAGEBOX1;
    d->skinMax_ = NATIVE_BITMAP_MESSAGEBOX1;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_LISTBOX)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_MESSAGEBOX1; // does not matter, it has no own bitmap
    d->skinMin_ = NATIVE_BITMAP_MESSAGEBOX1;
    d->skinMax_ = NATIVE_BITMAP_MESSAGEBOX1;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_MATCH_RESULTS)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_MESSAGEBOX1; // does not matter, it has no own bitmap
    d->skinMin_ = NATIVE_BITMAP_MESSAGEBOX1;
    d->skinMax_ = NATIVE_BITMAP_MESSAGEBOX1;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_MESSAGEBOXYESNO)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_MATCH_ENG; // does not matter, it has no own bitmap
    d->skinMin_ = NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_MATCH_ENG;
    d->skinMax_ = NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_MATCH_ENG;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;

    d = &descriptor[static_cast<size_t>(GameObjectInterface::Type::GOT_MESSAGEBOXTERMSOFUSE)][static_cast<size_t>(State::Type::ST_IDLE)];
    d->skinStart_ = NATIVE_BITMAP_MESSAGEBOX_TERMS_OF_USE;
    d->skinMin_ = NATIVE_BITMAP_MESSAGEBOX_TERMS_OF_USE;
    d->skinMax_ = NATIVE_BITMAP_MESSAGEBOX_TERMS_OF_USE;
    d->animTimeStep_ = 1000./7.;
    d->yPosCorrection_ = 0;
    d->velocity_ = 0.f;
}
