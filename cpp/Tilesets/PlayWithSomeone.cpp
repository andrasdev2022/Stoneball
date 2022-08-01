#include "PlayWithSomeone.h"
#include "Application.h"
#include "World.h"
#include "Environment.h"
#include "Screen.h"
#include "BouncingStones.h"
#include "EnumClassOp.h"
#include "MediaData.h"
#include "SoundEffect.h"
#include "Messagebox1.h"
#include "BitmapIds.h"
#include "MatchResults.h"
#include "StringUtil.h"
#include "Bitmap565.h"

#ifdef __SERVERAPP__
#include "MultiplayerWorld.h"
#endif

using namespace Application;
using namespace StringUtil;

namespace {
int toInt(PlayWithSomeone::Team team) {
    return static_cast<int>(team);
}

void killMessageBoxes() noexcept {
    for(auto& w : latestWorld()->otherElementList()) {
        if(w.second->messagebox1()) {
            w.second->die();
        }
    }
}
} // namespace

PlayWithSomeone::PlayWithSomeone(int teamCount)
: Tileset()
, TilesetCardHandler(teamCount)
, teamCount_(static_cast<Team>(teamCount))
, activeTeam_(Team::LAST_ONE) {
    for(int i = 0; i < teamCount; ++i) {
        elapsedGameTime_.push_back(0.);
        latestTurnTimeStamp_.push_back(0.);
        teamNames_.push_back(L"");
    }
}

std::vector<int> PlayWithSomeone::calcScore() noexcept {
    std::vector<int> teams(toInt(teamCount_));

    for(const auto& kP : latestWorld()->knightList()) {
        const auto& knight = kP.second;
        if(knight->type() == GameObject::Type::GOT_KNIGHT_RED) {
            teams[0] += environment()->score(knight->id());
        } else if(knight->type() == GameObject::Type::GOT_KNIGHT_DARK) {
            teams[1] += environment()->score(knight->id());
        } else if(knight->type() == GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE) {
            teams[2] += environment()->score(knight->id());
        } else if(knight->type() == GameObject::Type::GOT_KNIGHT_GREEN_FEMALE) {
            teams[3] += environment()->score(knight->id());
        }
    }
    return teams;
}

std::wstring PlayWithSomeone::prepareTimeString(int playerIndex) noexcept {
    int minutes{};
    int seconds{};

    if(latestWorld()->multiplayer()) {
        minutes = floor(elapsedGameTime_[playerIndex] / 6e4);
        seconds = floor(elapsedGameTime_[playerIndex] / 1e3) - minutes * 60;
    } else {
        minutes = floor((Application::playTime() * 6.e4 - elapsedGameTime_[playerIndex]) / 6e4);
        seconds = floor((Application::playTime() * 6.e4 - elapsedGameTime_[playerIndex]) / 1e3) - minutes * 60;
    }

    if(minutes < 0) {
        minutes = 0;
        seconds = 0;
    }
    if(seconds < 0) seconds = 0;

    auto minutesStr = std::to_wstring(minutes);
    if(minutesStr.length() == 1) {
        minutesStr = L"0" + minutesStr;
    }
    auto secondsStr = std::to_wstring(seconds);
    if(secondsStr.length() == 1) {
        secondsStr = L"0" + secondsStr;
    }
    return minutesStr + L":" + secondsStr;
}

uint16_t PlayWithSomeone::elapsedGameTime(uint8_t team) noexcept {
    if(toInt(teamCount_) <= team) return 0;
    return (uint16_t)(elapsedGameTime_[team] / 1000);
}

void PlayWithSomeone::setElapsedGameTime(uint8_t team, uint16_t value) noexcept {
    if(toInt(teamCount_) <= team) return;
    elapsedGameTime_[team] = 1e3 * (double)value;
}

void PlayWithSomeone::setTeamName(uint8_t team, std::wstring name) noexcept {
    if(toInt(teamCount_) <= team) return;
    teamNames_[team] = trim(name);
}

// HeroWalkStrategy must not be started if a hero pushed a stone in this turn
bool PlayWithSomeone::walkHistoryRuleCheck() const noexcept {
    bool push = std::find_if(moveHistory_.events().begin(), moveHistory_.events().end(), [](auto& event){
        return event.type == MoveHistory::EventType::PUSH;
    }) != moveHistory_.events().end();
    return !push;
}

void PlayWithSomeone::resetWatch() noexcept {
    for(int i = 0; i < toInt(teamCount_); ++i) {
        elapsedGameTime_[i] = 0.;
    }
}

void PlayWithSomeone::drawTexts() noexcept {
#ifndef __SERVERAPP__
    auto& f = Application::font();
    bool strikethrough = false;
    if(std::find(quitters_.begin(), quitters_.end(), (uint8_t)GameObjectInterface::Type::GOT_KNIGHT_RED) != quitters_.end()) {
        strikethrough = true;
    }
    (*f).setColor(200, 0, 0);
    (*f).draw(Application::screen(), textXPos_[0], textYPos_, trim(textRedTeam_), strikethrough);

    strikethrough = false;
    if(std::find(quitters_.begin(), quitters_.end(), (uint8_t)GameObjectInterface::Type::GOT_KNIGHT_DARK) != quitters_.end()) {
        strikethrough = true;
    }
    (*f).setColor(50, 50, 50);
    (*f).draw(Application::screen(), textXPos_[1], textYPos_, trim(textGrayTeam_), strikethrough);

    strikethrough = false;
    if(textBlondeTeam_.length()) {
        if(std::find(quitters_.begin(), quitters_.end(), (uint8_t)GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE) != quitters_.end()) {
            strikethrough = true;
        }
        (*f).setColor(0xe6, 0xd8, 0x6c);
        (*f).draw(Application::screen(), textXPos_[2], textYPos_, trim(textBlondeTeam_), strikethrough);
    }

    strikethrough = false;
    if(textGreenTeam_.length()) {
        if(std::find(quitters_.begin(), quitters_.end(), (uint8_t)GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE) != quitters_.end()) {
            strikethrough = true;
        }
        (*f).setColor(0x87, 0xcb, 0x00);
        (*f).draw(Application::screen(), textXPos_[3], textYPos_, trim(textGreenTeam_), strikethrough);
    }

    auto& tr = Application::translator();
    auto& f12px = Application::font12px();

    if(latestWorld()->multiplayer()) {
        (*f12px).setColor(50, 50, 50);
        (*f12px).draw(Application::screen(), screenPosX() + 5_ZRD, screenPosY() + 48_ZRD, (*tr)(TranslateInterface::Key::TRA__ROOM_NAME) + L": ");
        (*f12px).draw(Application::screen(), screenPosX() + 5_ZRD, screenPosY() + 57_ZRD, Application::roomName());
        if(viewOnly()) {
            (*f12px).draw(Application::screen(), screenPosX() + 5_ZRD, screenPosY() + 66_ZRD, L"(v)");
        }
        if(Application::viewerCount() > 0) {
            constexpr int id = NATIVE_BITMAP_EYE;
            const int x = 5_ZRD;
            const int y = 80_ZRD;
            MediaData* data = bitmapData(id, false);
            Rect rect(0, 0, bitmapWidth(id), bitmapHeight(id));
            uint16_t* dst = Application::screen()->content() + screenWidth() * y + x;
            Bitmap565::copyIntersectionRarematrix(dst, rect, screenWidth(), data->data(), rect);
            (*f12px).draw(Application::screen(), screenPosX() + 23_ZRD, screenPosY() + 78_ZRD, std::to_wstring(Application::viewerCount()));
        }
    }

    if(activeTeam_ != Team::LAST_ONE) {
        //updateElapsedTime();

        //if(gameOver()) return;

        for(int i = 0; i < toInt(teamCount_); ++i) {
            if(elapsedGameTime_[i] > 65534000. || elapsedGameTime_[i] < 0.) {
                 elapsedGameTime_[i] = 0.;
            }
        }

        (*f12px).setColor(200, 0, 0);
        (*f12px).draw(Application::screen(), textXPos_[0], textYPos_ + textHeight_ - 2_ZRD, prepareTimeString(0));

        (*f12px).setColor(50, 50, 50);
        (*f12px).draw(Application::screen(), textXPos_[1], textYPos_ + textHeight_ - 2_ZRD, prepareTimeString(1));

        if(textBlondeTeam_.length()) {
            (*f12px).setColor(0xe6, 0xd8, 0x6c);
            (*f12px).draw(Application::screen(), textXPos_[2], textYPos_ + textHeight_ - 2_ZRD, prepareTimeString(2));
        }

        if(textGreenTeam_.length()) {
            (*f12px).setColor(0x87, 0xcb, 0x00);
            (*f12px).draw(Application::screen(), textXPos_[3], textYPos_ + textHeight_ - 2_ZRD, prepareTimeString(3));
        }
    }
#endif
}

void PlayWithSomeone::followTheKnight() noexcept {
    // I'll create a dummy game object in order to reposition the screen
    int positionY = position().y;
    if(heightInPixels() > (int)screenHeight()) {
        positionY += heightInPixels() / 2;
    } else {
        positionY += screenHeight() / 2;
    }
    int positionX = position().x;
    if(widthInPixels() > (int)screenWidth()) {
        positionX += widthInPixels() / 2;
    } else {
        positionX += screenWidth() / 2;
    }
    GrayBouncingStone gbs(positionX, positionY);
    gbs.setX(gbs.x() - gbs.width() / 2);
    gbs.setY(gbs.y() - gbs.height() / 2);
    screen()->follow(&gbs, environment()->gameMap());
}

bool PlayWithSomeone::isBlockingTile(int x, int y, VectorD direction /*= VectorD()*/) const noexcept {
    if(Tileset::isBlockingTile(x, y, direction)) return true;
    if(Application::playMode() != Application::PlayMode::TURNBASED) return false;

    // The knight must not turn left/right and must not go back after he pushed a ball in this turn
    // 0. Turn handling
    //    Events to record: which knight did any action
    //                      which direction did he move a ball
    // 1. Did he push a ball?
    // 2. If so, does he want to step left/right/back?

    if(!historySize()) return false;

    if(!direction.isNull()) {
        //std::cout << "PlayWithFriend3::isBlockingTile: " << direction.x << ", " << direction.y << "\n";
        for(auto& e : moveHistory_.events()) {
            if(e.type == MoveHistory::EventType::PUSH && (abs(direction.x - e.direction.x) > 1e-6 || abs(direction.y - e.direction.y) > 1e-6)) {
                return true;
            }
        }
    }
    // Besides, if a knight did any action, another one cannot be selected in this turn
    // implementation: WorldTouchEvent.cpp
    return false;
}

void PlayWithSomeone::correctTimestamp() noexcept {
    if(latestWorld()->multiplayer()) {
#ifdef __SERVERAPP__
        EnumClassOp idx(activeTeam_);
        latestTurnTimeStamp_[idx] = latestWorld()->multiplayer()->snapshotTimeStamp();
#endif
    } else {
        EnumClassOp idx(activeTeam_);
        latestTurnTimeStamp_[idx] = Application::now();
    }
}

void PlayWithSomeone::updateElapsedTime(double now) noexcept {
    if(gameOver()) return;

    if(activeTeam_ == Team::LAST_ONE) {
        resetWatch();
        return;
    } else {
#ifdef __SERVERAPP__
        if(!playersJoined() && environment()->activeKnightId() == Environment::NONE_ACTIVE_KNIGHT) {
            resetWatch();
        }
#endif
    }
    EnumClassOp idx(activeTeam_);
    if(latestWorld()->multiplayer()) {
#ifdef __SERVERAPP__
        if(latestTurnTimeStamp_[idx] < 1e-5) {
            latestTurnTimeStamp_[idx] = latestWorld()->multiplayer()->snapshotTimeStamp();
        }

        // before we modify the elapsed time, we hav to check if the game is over
        if(gameOver()) return;
        if(testSuddenDeath()) {
            return;
        }

        elapsedGameTime_[idx] += latestWorld()->multiplayer()->snapshotTimeStamp() - latestTurnTimeStamp_[idx];
        latestTurnTimeStamp_[idx] = latestWorld()->multiplayer()->snapshotTimeStamp();
#endif // __SERVERAPP__
    } else {
        if(latestTurnTimeStamp_[idx] < 1e-5) {
            latestTurnTimeStamp_[idx] = now;
        }
        //debug("updateElapsedTime DBG 1");
        if(gameOver()) return;
        //debug("updateElapsedTime DBG 2");
        if(testSuddenDeath()) {
            return;
        }
        //debug("updateElapsedTime DBG 3");
        elapsedGameTime_[idx] += now - latestTurnTimeStamp_[idx];
        latestTurnTimeStamp_[idx] = now;
    }
}

void PlayWithSomeone::teamCountCheck() noexcept {
    if(activeTeam_ != Team::LAST_ONE) {
#ifdef __SERVERAPP__
        if(!playersJoined_ && Application::playerCount() == (size_t)teamCount_) {
            playersJoined_ = true;
            auto messagebox = new Messagebox1(1.5e3, TranslateInterface::Key::TRA__GAME_HAS_BEGUN);
            Application::showMessageBox(messagebox);
            Application::setFurtherPlayersViewOnly(true);
        }
#endif
    }
}

[[nodiscard]] bool PlayWithSomeone::isKnightInActiveTeam(Knight* k) const noexcept {
    if(activeTeam_ == Team::RED_TEAM && k->type() == GameObject::Type::GOT_KNIGHT_RED) return true;
    if(activeTeam_ == Team::DARK_TEAM && k->type() == GameObject::Type::GOT_KNIGHT_DARK) return true;
    if(activeTeam_ == Team::BLONDE_FEMALE_TEAM && k->type() == GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE) return true;
    if(activeTeam_ == Team::GREEN_FEMALE_TEAM && k->type() == GameObject::Type::GOT_KNIGHT_GREEN_FEMALE) return true;
    return false;
}

void PlayWithSomeone::setActiveTeam(GameObjectInterface::Type type) noexcept {
    if(type == GameObject::Type::GOT_KNIGHT_RED) {
        activeTeam_ = Team::RED_TEAM;
    } else if(type == GameObject::Type::GOT_KNIGHT_DARK) {
        activeTeam_ = Team::DARK_TEAM;
    } else if(type == GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE) {
        activeTeam_ = Team::BLONDE_FEMALE_TEAM;
    } else if(type == GameObject::Type::GOT_KNIGHT_GREEN_FEMALE) {
        activeTeam_ = Team::GREEN_FEMALE_TEAM;
    }
    //LOGI("PlayWithFriend3::setActiveTeam %d", (int)activeTeam_);
}

void PlayWithSomeone::setActiveTeamId(unsigned char teamId) noexcept {
    //debug("PlayWithSomeone::setActiveTeamId before %d", (int)activeTeam_);
    if(toInt(teamCount_) > teamId) {
        EnumClassOp<Team> oo;
        oo = teamId;
        if(activeTeam_ != oo) {
            //resetWatch();
            correctTimestamp();
        }
        activeTeam_ = oo;
    }
    //debug("PlayWithSomeone::setActiveTeamId after %d", (int)activeTeam_);
}

unsigned char PlayWithSomeone::activeTeamId() const noexcept {
    return static_cast<unsigned char>(activeTeam_);
}

void PlayWithSomeone::calcTileMapRect() {
    width_ = tileMap_.find(nextLine) + 1;
    height_ = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [this](const wchar_t& c){if(c == nextLine) ++height_;});
    // with_ - 1, because '\n' is counted in the with, but it is not visible
    if((int)screenHeight() > heightInPixels()) {
        position_.y = ((int)screenHeight() - heightInPixels()) / 2;
    }
    if((int)screenWidth() > widthInPixels()) {
        position_.x = ((int)screenWidth() - widthInPixels()) / 2;
    }
    //std::cout << "Tileset::calcSizeByTileMap: screenHeight(): " << screenHeight() << ", height: " << height_ * tileSize_.height
    //          << ", position_.y: " << position_.y << "\n";
}

void PlayWithSomeone::stopServer() noexcept {
#ifdef __SERVERAPP__
    Application::stopServerAt(Application::now() + 5e3);
#endif
}

// server side
bool PlayWithSomeone::testSuddenDeath() noexcept {
    if(gameOver()) return true;

    for(EnumClassOp i = Team::RED_TEAM; i < teamCount_; ++i) {
        if((uint16_t)(elapsedGameTime_[i] / 1000) > 60 * Application::playTime()) {
            announceWinner(i);
            break;
        }
    }

    if(gameOver()) {
        stopServer();
    }
    return gameOver();
}

void PlayWithSomeone::nextTurn() noexcept {
    if(gameOver()) return;

    // Active knight must sit down
    auto it = latestWorld()->knightList().find(environment()->activeKnightId());
    if(it != latestWorld()->knightList().end()) {
        if(it->second->isRunning()) {
            return;
        }
        it->second->sitDown();
    } else {
        // there is no selected team member
    }

    // before we change the active knight id, we have to check wether it is a pass or a real step
    if(Application::environment()->activeKnightId() != Environment::NONE_ACTIVE_KNIGHT) {
        passCount_ = 0;
    } else {
        if(std::find(quitters().begin(), quitters().end(), activeTeamId()) == quitters().end()) {
            ++passCount_;
        }
    }

    if((!latestWorld()->multiplayer() && passCount_ == (unsigned)teamCount_) ||
    (latestWorld()->multiplayer() && passCount_ == Application::playerCount()) ||
    lastTurn_) {
        announceWinner(-1);
        SoundEffect::play(SoundEffect::Name::PLAY_DRUMS);
        return;
    }

    Application::environment()->setActiveKnightId(Environment::NONE_ACTIVE_KNIGHT);

    // Next team must be activated
    EnumClassOp op(activeTeam_);

    do {
        activeTeam_ = ++op % static_cast<Team>(teamCount_);
    } while(std::find(quitters().begin(), quitters().end(), activeTeamId()) != quitters().end());

    clearHistory();
    showNextTurnMessageBox();

    correctTimestamp();
    updateElapsedTime(Application::now());
    SoundEffect::play(SoundEffect::Name::PLAY_DRUMS);
}

void PlayWithSomeone::showNextTurnMessageBox() noexcept {
    // The content of the messagebox must be stored fully in the Translator in order the client
    // can translate to his own language
    TranslateInterface::Key message = TranslateInterface::Key::TRA__SOMEONES_TURN;
    std::wstring name;
    switch(activeTeam_) {
    case Team::RED_TEAM: name = teamNames_[0]; break;
    case Team::DARK_TEAM: name = teamNames_[1]; break;
    case Team::BLONDE_FEMALE_TEAM: name = teamNames_[2]; break;
    case Team::GREEN_FEMALE_TEAM: name = teamNames_[3]; break;
    case Team::LAST_ONE: break;
    }

    if(!name.length()) {
        switch(activeTeam_) {
        case Team::RED_TEAM: message = TranslateInterface::Key::TRA__RED_TURN; break;
        case Team::DARK_TEAM: message = TranslateInterface::Key::TRA__DARK_TURN; break;
        case Team::BLONDE_FEMALE_TEAM: message = TranslateInterface::Key::TRA__BLONDE_TURN; break;
        case Team::GREEN_FEMALE_TEAM: message = TranslateInterface::Key::TRA__GREEN_TURN; break;
        case Team::LAST_ONE: break;
        }
    }

    // Let's pop up a messagebox here
    BlueTriangleUPtr messagebox(new Messagebox1(1000, message, name));
    latestWorld()->addOtherElement(std::move(messagebox));
}

void PlayWithSomeone::setWinnerTeam(uint8_t team) noexcept {
    //debug("PlayWithSomeone::setWinnerTeam: %u", team);
    winnerTeam_ = team;
}

void PlayWithSomeone::announceWinner(int looserIndex) noexcept {
    auto score = calcScore();
    MatchResultsUPtr matchResults = MatchResultsUPtr(new MatchResults(static_cast<uint8_t>(teamCount_)));

    auto nomalizeTime = [](uint16_t e) -> uint16_t {
        if(Application::playTime() * 60 > e) return Application::playTime() * 60 - e;
        return 0;
    };

    switch(static_cast<uint8_t>(teamCount_)) {
    case 2:
        matchResults->setScores(score[0], score[1], 0, 0);
        matchResults->setElapsedGameTimes(nomalizeTime(elapsedGameTime(0)),
                nomalizeTime(elapsedGameTime(1)),
                0,
                0);
        break;
    case 3:
        matchResults->setScores(score[0], score[1], score[2], 0);
        matchResults->setElapsedGameTimes(nomalizeTime(elapsedGameTime(0)),
                nomalizeTime(elapsedGameTime(1)),
                nomalizeTime(elapsedGameTime(2)),
                0);
        break;
    case 4:
        matchResults->setScores(score[0], score[1], score[2], score[3]);
        matchResults->setElapsedGameTimes(nomalizeTime(elapsedGameTime(0)),
                nomalizeTime(elapsedGameTime(1)),
                nomalizeTime(elapsedGameTime(2)),
                nomalizeTime(elapsedGameTime(3)));
        break;
    default: debug("ERROR: PlayWithSomeone::announceWinner Too high team count"); break;
    }

    if(looserIndex != -1) {
        matchResults->setLooserTeam(looserIndex);
    }
    setWinnerTeam(matchResults->winnerTeam());

    debug("winner team: %u", winnerTeam_);
    stopServer();
#ifdef __SERVERAPP__
    killMessageBoxes();
    Application::showWidget(matchResults.release());
#endif
    if(!latestWorld()->multiplayer()) {
        killMessageBoxes();
        Application::showWidget(matchResults.release());
    }
}

std::wstring PlayWithSomeone::generateYourTeamText() const noexcept {
    auto& tr = Application::translator();
    auto yourTeamText = (*tr)(TranslateInterface::Key::TRA__YOUR_TEAM);
    if(Application::profileName().length() > 0) {
        yourTeamText = Application::profileName();
    }
    return yourTeamText;
}

void PlayWithSomeone::generateRedTeamText(const std::vector<int>& teams) noexcept {
    auto& tr = Application::translator();
    if(teamNames_[0].length() > 0) {
        textRedTeam_ = teamNames_[0] + L": " + std::to_wstring(teams[0]);
    } else {
        textRedTeam_ = (*tr)(TranslateInterface::Key::TRA__RED_TEAM) + L": " + std::to_wstring(teams[0]);
    }
}

void PlayWithSomeone::generateGrayTeamText(const std::vector<int>& teams) noexcept {
    auto& tr = Application::translator();
    if(teamNames_[1].length() > 0) {
        textGrayTeam_ = teamNames_[1] + L": " + std::to_wstring(teams[1]);
    } else {
        textGrayTeam_ = (*tr)(TranslateInterface::Key::TRA__DARK_TEAM) + L": " + std::to_wstring(teams[1]);
    }
}

void PlayWithSomeone::generateBlondeTeamText(const std::vector<int>& teams) noexcept {
    auto& tr = Application::translator();
    if(teamNames_[2].length() > 0) {
        textBlondeTeam_ = teamNames_[2] + L": " + std::to_wstring(teams[2]);
    } else {
        textBlondeTeam_ = (*tr)(TranslateInterface::Key::TRA__BLONDE_TEAM) + L": " + std::to_wstring(teams[2]);
    }
}

void PlayWithSomeone::generateGreenTeamText(const std::vector<int>& teams) noexcept {
    auto& tr = Application::translator();
    if(teamNames_[3].length() > 0) {
        textGreenTeam_ = teamNames_[3] + L": " + std::to_wstring(teams[3]);
    } else {
        textGreenTeam_ = (*tr)(TranslateInterface::Key::TRA__GREEN_TEAM) + L": " + std::to_wstring(teams[3]);
    }
}
