#include "PlayWithFriend3.h"
#include "Application.h"
#include "Environment.h"
#include "MediaData.h"
#include "EnumClassOp.h"
#include "BouncingStones.h"
#include "Screen.h"
#include "Messagebox1.h"
#include "BitmapIds.h"
#include "TouchEventHandler.h"
#include "CardPull.h"
#include "CardPushPlayer.h"
#include "CardTeleport.h"

#ifdef __SERVERAPP__
#include "MultiplayerWorld.h"
#endif

using namespace Application;

PlayWithFriend3::PlayWithFriend3()
: PlayWithSomeone(TEAMCOUNT) {
    tileMap_ =
           //0123456789012345678901234
           L"------------------------------\n" // 0
            "------------------------------\n" // 1
            "---------************---------\n" // 2
            "---------*X   XX   x*---------\n" // 3
            "---------*X        x*---------\n" // 4
            "---------*  OKHhOo  *---------\n" // 5
            "---------*  o ó  K  *---------\n" // 6
            "---------*X K  ó H x*---------\n" // 7
            "---------*X H ó  h x*---------\n" // 8
            "---------*  h  ó o  *---------\n" // 9
            "---------*  oOhKHO  *---------\n" // 0
            "---------*X        x*---------\n" // 1
            "---------*X   xx   x*---------\n" // 2
            "---------************---------\n" // 3
            "------------------------------\n" // 4
            "------------------------------\n" // 5 // for the extreme screen aspect ratios
            "------------------------------\n" // 6
;
}

void PlayWithFriend3::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
           L"                              \n" // 5
            "                              \n" // 6
            "        d            A        \n" // 0
            "          eqeqeqeqeq          \n" // 1
            "        a eeeeeeeeee          \n" // 2
            "          eqeqeqeqeq          \n" // 3
            "          eeeeeeeeee d        \n" // 4
            "          eqeqeqeqeq          \n" // 5
            "          eeeeeeeeee          \n" // 6
            "        A eqeqeqeqeq          \n" // 7
            "          eeeeeeeeee          \n" // 8
            "          eqeqeqeqeq a        \n" // 9
            "          eeeeeeeeee          \n" // 0
            "                              \n" // 1
            "                              \n" // 2
            "                              \n" // 3 // for the extreme screen aspect ratios
            "                              \n" // 4
           //0123456789012345678901234567
            ;
;

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    setPlayMode(Application::PlayMode::TURNBASED);

#ifdef __SERVERAPP__
    initDecks(world, 100, 100, 100);
#else
    if(!world->multiplayer()) {
        initDecks(world, 100, 100, 100);
    }
#endif
}

void PlayWithFriend3::updateTexts([[maybe_unused]]double now) noexcept {
    auto&& teams = calcScore();

    auto& tr = Application::translator();
    if(latestWorld()->multiplayer()) {
        if(viewOnly()) {
            generateRedTeamText(teams);
            generateGrayTeamText(teams);
            generateBlondeTeamText(teams);
        } else {
            auto&& yourTeamText = generateYourTeamText();
            if(Application::avatar() == Application::Avatar::RED) {
                textRedTeam_ = yourTeamText + L": " + std::to_wstring(teams[0]);
                generateGrayTeamText(teams);
                generateBlondeTeamText(teams);
            } else if(Application::avatar() == Application::Avatar::DARK) {
                generateRedTeamText(teams);
                textGrayTeam_ = yourTeamText + L": " + std::to_wstring(teams[1]);
                generateBlondeTeamText(teams);
            } else if(Application::avatar() == Application::Avatar::BLONDE_FEMALE) {
                generateRedTeamText(teams);
                generateGrayTeamText(teams);
                textBlondeTeam_ = yourTeamText + L": " + std::to_wstring(teams[2]);
            }
        }
    } else {
        textRedTeam_ = (*tr)(TranslateInterface::Key::TRA__RED_TEAM) + L": " + std::to_wstring(teams[0]);
        textGrayTeam_ = (*tr)(TranslateInterface::Key::TRA__DARK_TEAM) + L": " + std::to_wstring(teams[1]);
        textBlondeTeam_ = (*tr)(TranslateInterface::Key::TRA__BLONDE_TEAM) + L": " + std::to_wstring(teams[2]);
    }

    scoreText_ = textRedTeam_ + textGrayTeam_ + textBlondeTeam_;
    auto& f = Application::font();
    //int textWidthHalf = (*f).width(txt1);
    int teamTextDistance = 25_ZRD;
    textHeight_ = (*f).height(scoreText_);
    textYPos_ = std::max(position().y + tileHeight(), (int)screenPosY() + 1_ZRD);//5_ZRH + screenPosY();
    textXPos_[0] = 0;
    textXPos_[1] = textXPos_[0] + (*f).width(textRedTeam_) + teamTextDistance;
    textXPos_[2] = textXPos_[1] + (*f).width(textGrayTeam_) + teamTextDistance;
    updateMinimumTextXDistance();
    textWidth_ = textXPos_[2] + (*f).width(textBlondeTeam_);
    int newTextXPos0Diff = (screenWidth() - textWidth_) / 2 + screenPosX();
    textXPos_[2] += newTextXPos0Diff;
    textXPos_[1] += newTextXPos0Diff;
    textXPos_[0] += newTextXPos0Diff;
}

// multiplayer client must not run the updateTeamMarker()
void PlayWithFriend3::updateTeamMarker([[maybe_unused]]double now) noexcept {
    if(gameOver()) return;
    auto& otherElementList = latestWorld()->otherElementList();
    auto it = otherElementList.find(bouncingStoneId_);
    if(activeTeam_ != Team::LAST_ONE) {
        //LOGI("PlayWithFriend3::updateTeamMarker");
        BlueTriangleUPtr bt;
        const int yPos = textYPos_ - 5_ZRD;
        const int xCorrection = -27_ZRD;
        if(it != otherElementList.end() && lastActiveTeam_ != activeTeam_) {
            it->second->die();
        }
        if(it == otherElementList.end() || lastActiveTeam_ != activeTeam_) {
            if(activeTeam_ == Team::RED_TEAM) {
                bt.reset(new RedBouncingStone(textXPos_[0] + xCorrection, yPos));
                bt->setX(textXPos_[0] + xCorrection);
            } else if(activeTeam_ == Team::DARK_TEAM) {
                bt.reset(new GrayBouncingStone(textXPos_[1] + xCorrection, yPos));
                bt->setX(textXPos_[1] + xCorrection);
            } else if(activeTeam_ == Team::BLONDE_FEMALE_TEAM) {
                bt.reset(new YellowBouncingStone(textXPos_[2] + xCorrection, yPos));
                bt->setX(textXPos_[2] + xCorrection);
            }
            bt->setY(yPos);
            bouncingStoneId_ = bt->id();
            latestWorld()->addOtherElement(std::move(bt));
        } else {
            //debug("PlayWithFriend3::updateTeamMarker %d %d %d", textXPos_, textXPos_[1], textXPos_[2]);
            if(activeTeam_ == Team::RED_TEAM) {
                it->second->setX(textXPos_[0] + xCorrection);
            } else if(activeTeam_ == Team::DARK_TEAM) {
                it->second->setX(textXPos_[1] + xCorrection);
            } else if(activeTeam_ == Team::BLONDE_FEMALE_TEAM) {
                it->second->setX(textXPos_[2] + xCorrection);
            }
            it->second->setY(yPos);
        }
    } else {
        if(it != otherElementList.end()) {
            it->second->die();
            bouncingStoneId_ = 0xffffffff;
        }
    }
    lastActiveTeam_ = activeTeam_;
}

uint32_t PlayWithFriend3::score() const noexcept {
    uint32_t teams[3]{0,0,0};

    for(const auto& kP : latestWorld()->knightList()) {
        const auto& knight = kP.second;
        if(knight->type() == GameObject::Type::GOT_KNIGHT_RED) {
            teams[0] += environment()->score(knight->id());
        } else if(knight->type() == GameObject::Type::GOT_KNIGHT_DARK) {
            teams[1] += environment()->score(knight->id());;
        } else if(knight->type() == GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE) {
            teams[2] += environment()->score(knight->id());;
        }
    }

    //debug("PlayWithFriend3::score: %u %u %u", teams[0], teams[1], teams[2]);
    return (((teams[0] << 8) | teams[1]) << 8) | teams[2];
}

void PlayWithFriend3::setScore(uint32_t score) noexcept {
    uint16_t redScore = score >> 16;
    uint16_t darkScore = (score & 0xff00) >> 8;
    uint16_t blondeScore = score & 0xff;

    uint32_t oldScore = this->score();
    uint16_t oldRedScore = oldScore >> 16;
    uint16_t oldDarkScore = (oldScore & 0xff00) >> 8;
    uint16_t oldBlondeScore = oldScore & 0xff;

    //debug("PlayWithFriend3::setScore: %u %u %u", redScore, darkScore, blondeScore);
    //debug("PlayWithFriend3::setScore: %u %u %u", oldRedScore, oldDarkScore, oldBlondeScore);

    uint32_t redKnightId    = 0xffffffff;
    uint32_t darkKnightId   = 0xffffffff;
    uint32_t blondeKnightId = 0xffffffff;
    for(const auto& kP : latestWorld()->knightList()) {
        const auto& knight = kP.second;
        if(knight->type() == GameObject::Type::GOT_KNIGHT_RED && redKnightId == 0xffffffff) {
            redKnightId = knight->id();
        } else if(knight->type() == GameObject::Type::GOT_KNIGHT_DARK && darkKnightId == 0xffffffff) {
            darkKnightId = knight->id();
        } else if(knight->type() == GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE && blondeKnightId == 0xffffffff) {
            blondeKnightId = knight->id();
        } else if(redKnightId != 0xffffffff && darkKnightId != 0xffffffff && blondeKnightId != 0xffffffff) {
            break;
        }
    }

    while(oldRedScore < redScore) {
        //debug("incScore(redKnightId): %u, score: %d", redKnightId, environment()->score(redKnightId));
        environment()->incScore(redKnightId);
        //debug("score: %d", environment()->score(redKnightId));
        ++oldRedScore;
    }

    while(oldDarkScore < darkScore) {
        environment()->incScore(darkKnightId);
        ++oldDarkScore;
    }

    while(oldBlondeScore < blondeScore) {
        environment()->incScore(blondeKnightId);
        ++oldBlondeScore;
    }
}
