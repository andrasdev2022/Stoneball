#include "MatchResults.h"
#include "Screen.h"
#include "MediaData.h"
#include "Memory.h"
#include "Bitmap565.h"
#include "Application.h"
#include "BitmapIds.h"
#include <algorithm>
#include "Global.h"
#include "Serialize.h"
#include "UDPMessageData.h"
#include <array>
#include "Tileset.h"
#include "Algorithms.h"
#include "MemCache.h"
#include "Fireworks.h"
#include "World.h"
#include "Trophy.h"

namespace {
    constexpr uint16_t cacheBlockSize_ = 100;
    MemCacheInterfaceUInt16 tableCache_;
    uint16_t cacheStartLine_{};
    uint16_t tableCacheStartLine_{};
    uint8_t fireworksStarted_{};
}

MatchResults::MatchResults(uint8_t teamNum)
: BlueTriangle(0, 0, GameObject::Type::GOT_MATCH_RESULTS)
, teamNum_(teamNum)
{
    width_ = 342_ZRD;
    height_ = 229_ZRD;
    tableCache_.reset();
    cacheStartLine_ = 0;
    tableCacheStartLine_ = 0;
    fireworksStarted_ = 0;
}

BlueTriangle* MatchResults::copy() const noexcept {
    return new MatchResults(*this);
}

double MatchResults::z() const noexcept {
    return 2000. + GameObject::z();
}

void MatchResults::update(double now) noexcept {
    BlueTriangle::update(now);
    if(!fireworksCount_ && Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
        Application::showWidget(new Fireworks(0, 0, GameObjectInterface::Type::GOT_FIREWORK_LONG_BLUE));
        Application::showWidget(new Fireworks(0, 0, GameObjectInterface::Type::GOT_FIREWORK_LONG_ORANGE));
        ++fireworksCount_;
        debug("MatchResults::update firework");
    }
}

void MatchResults::updateTeamDatas() noexcept {
    teamDatas_.clear();
    teamDatas_.push_back({0, redScore_, redElapsedTime_});
    teamDatas_.push_back({1, grayScore_, grayElapsedTime_});
    if(teamNum_ > 2) {
        teamDatas_.push_back({2, yellowScore_, yellowElapsedTime_});
    }
    if(teamNum_ > 3) {
        teamDatas_.push_back({3, greenScore_, greenElapsedTime_});
    }
    std::sort(teamDatas_.begin(), teamDatas_.end(), TeamData::greater);
    if(appearenceTime_ < 1e-5) {
        appearenceTime_ = Application::now();
    }
    if(looserTeam_ != 255) {
        setLooserTeam(looserTeam_);
    }
}

void MatchResults::setLooserTeam(uint8_t teamId) noexcept {
    looserTeam_ = teamId;
    for(size_t i = 0; i < teamDatas_.size(); ++i) {
        if(teamDatas_[i].teamId == teamId && i != teamDatas_.size() - 1) {
            std::swap(teamDatas_[i], teamDatas_[i + 1]);
        }
    }
}

void MatchResults::drawLabel(Screen* screen, BitmapId bitmapId, int x, int y) noexcept {
    const MediaData* bData = bitmapData(bitmapId, false);
    Bitmap565::copyIntersection(screen->content(),
                                screen->rect(),
                                screen->rect().width,
                                bData->data(),
                                Rect(x, y, bData->width(), bData->height()),
                                bData->width(),
                                true,
                                100);
}

void MatchResults::drawLabelCentered(Screen* screen, BitmapId bitmapId, int y) noexcept {
    const MediaData* bData = bitmapData(bitmapId, false);
    Bitmap565::copyIntersection(screen->content(),
                                screen->rect(),
                                screen->rect().width,
                                bData->data(),
                                Rect(screenPosX() + (screen->width() - bData->width()) / 2, screenPosY() + y, bData->width(), bData->height()),
                                bData->width(),
                                true,
                                100);
}

std::wstring MatchResults::prepareTimeString(uint16_t elapsedGameTime) noexcept {
    int minutes{};
    int seconds{};

    if(Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING && elapsedGameTime > 0 &&
    elapsedGameTime != (uint16_t)Application::playTime() * 60) {
        --elapsedGameTime;
    }
    minutes = floor((double)elapsedGameTime / 6e1);
    seconds = floor((double)elapsedGameTime - 60. * minutes);

    if(minutes < 0) {
        minutes = Application::playTime();
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

void MatchResults::draw(Screen* screen) noexcept {
    // draw a dark transparent rectangle (342x229) in the middle of the screen
    //debug("screenPosY: %d, 0_ZRH: %d", (int)screenPosY(), (int)0_ZRH);
    tableX_ = screenPosX() + (screen->width() - width_) / 2.;
    tableY_ = (screen->height() - height_) / 2;
    uint16_t*  dstLine = screen->content() + (int)tableY_ * screen->width() + ((int)tableX_ - screen->x());

    const uint16_t color = Bitmap565::make565(0x31, 0x28, 0x18);

    int deltaX = Application::tileset()->deltaX();
    int deltaY = Application::tileset()->deltaY();
    if(tableCache_ && tableCache_->parameterCheck(tableX_, tableY_, deltaX, deltaY) && tableCache_->ready()) {
        tableCache_->copyWindow(dstLine);
    } else {
        for(uint16_t i = 0; i < 82_ZRD; ++i) {
            Memory::memset(dstLine, color, width_);
            dstLine += screen->width();
        }
        for(uint16_t i = 82_ZRD; i < height_; ++i) {
            for(uint16_t X = 0; X < width_; ++X) {
                dstLine[X] = Bitmap565::fastMix565(color, dstLine[X], 70);
            }
            dstLine += screen->width();
        }

        drawTableLabels(screen);
        drawKnights(screen);
    }

    saveTable2Cache(screen);
    initAnimation();
    drawAnimation();
}

void MatchResults::drawAnimation() noexcept {
    GameObject* firework{};
    for(auto& item : Application::latestWorld()->otherElementList()) {
        if(item.second->fireworks()) {
            if(!firework) {
                firework = item.second.get();
                item.second->setX(tableX_);
            } else {
                item.second->setX(tableX_ + width() - firework->width());
            }
            item.second->setY(tableY_ + screenPosY());
            if(Application::latestWorld()->multiplayer()) {
                item.second->update(Application::now());
            }
        } else if(item.second->trophy()) {
            item.second->setX(tableX_ + (width() - item.second->width()) / 2.);
            item.second->setY(tableY_ + screenPosY() + (height() - item.second->height()) / 2.);
            if(Application::latestWorld()->multiplayer()) {
                item.second->update(Application::now());
            }
        }
    }
}

void MatchResults::saveTable2Cache(Screen* screen) noexcept {
    int deltaX = Application::tileset()->deltaX();
    int deltaY = Application::tileset()->deltaY();

    if(!tableCache_ || !tableCache_->parameterCheck(tableX_, tableY_, deltaX, deltaY) || !tableCache_->ready()) {
        if(!tableCache_ || !tableCache_->parameterCheck(tableX_, tableY_, deltaX, deltaY)) {
            tableCache_ = std::make_unique<MemCache>();
            tableCache_->saveParameters(tableX_, tableY_, deltaX, deltaY);
            tableCache_->makeBuffer(screen->width(), width_, height_);
            tableCacheStartLine_ = 0;
        }
        uint16_t* dstBuffer = tableCache_->ptr() + tableCacheStartLine_ * width_;
        uint16_t* srcLine = screen->content() + (int)(tableY_ + tableCacheStartLine_) * screen->width() + ((int)tableX_ - screen->x());
        for(uint16_t i = 0; i < height_ - tableCacheStartLine_ && i < cacheBlockSize_; ++i) {
            memcpy(dstBuffer, srcLine, width_ * sizeof(uint16_t));
            dstBuffer += width_;
            srcLine += screen->width();
        }
        if(height_ <= tableCacheStartLine_ + cacheBlockSize_) {
            tableCache_->setReady(true);
        } else {
            tableCacheStartLine_ += cacheBlockSize_;
        }
    }
}

void MatchResults::drawKnights(Screen* screen) noexcept {
    auto& font12px = Application::font12px();
    (*font12px).setColor(0xdd, 0xc6, 0x9f);

    static const std::array<BitmapId, 4> knightIds{NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_2, NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_2,
                NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_2, NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_2};

    for(size_t i = 0; i < teamDatas_.size(); ++i) {
        const MediaData* knightData = bitmapData(knightIds[teamDatas_[i].teamId], false);
        Bitmap565::copyIntersectionRarematrix(screen->content(),
                                    screen->rect(),
                                    screen->rect().width,
                                    knightData->data(),
                                    Rect((int)tableX_ + 20_ZRD, screenPosY() + tableY_ + 75_ZRD + i * 35_ZRD, knightData->width(), knightData->height()));

        auto scoreString = std::to_wstring(teamDatas_[i].score);
        auto scoreWidth = (*font12px).width(scoreString);
        (*font12px).draw(screen, (int)tableX_ + 102_ZRD + (scoreLabelWidth_ - scoreWidth) / 2, screenPosY() + tableY_ + 90_ZRD + i * 35_ZRD, scoreString);
        auto timeString = prepareTimeString(teamDatas_[i].elapsedTime);
        auto timeWidth = (*font12px).width(timeString);
        (*font12px).draw(screen, (int)tableX_ + 252_ZRD + (timeLabelWidth_ - timeWidth)/ 2, screenPosY() + tableY_ + 90_ZRD + i * 35_ZRD, timeString);
    }
}

void MatchResults::drawTableLabels(Screen* screen) noexcept {
    scoreLabelWidth_ = 0;
    timeLabelWidth_ = 0;
    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawLabelCentered(screen, NATIVE_BITMAP_ENG_MATCH_RESULTS, tableY_ + 15_ZRD);
        drawLabel(screen, NATIVE_BITMAP_ENG_MATCH_RESULTS_SCORE, tableX_ + 102_ZRD, screenPosY() + tableY_ + 55_ZRD);
        drawLabel(screen, NATIVE_BITMAP_ENG_MATCH_RESULTS_TIME, tableX_ + 252_ZRD, screenPosY() + tableY_ + 55_ZRD);
        scoreLabelWidth_ = bitmapWidth(NATIVE_BITMAP_ENG_MATCH_RESULTS_SCORE);
        timeLabelWidth_ = bitmapWidth(NATIVE_BITMAP_ENG_MATCH_RESULTS_TIME);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawLabelCentered(screen, NATIVE_BITMAP_HUN_MATCH_RESULTS, tableY_ + 15_ZRD);
        drawLabel(screen, NATIVE_BITMAP_HUN_MATCH_RESULTS_SCORE, (int)tableX_ + 102_ZRD, screenPosY() + tableY_ + 55_ZRD);
        drawLabel(screen, NATIVE_BITMAP_HUN_MATCH_RESULTS_TIME, (int)tableX_ + 252_ZRD, screenPosY() + tableY_ + 55_ZRD);
        scoreLabelWidth_ = bitmapWidth(NATIVE_BITMAP_HUN_MATCH_RESULTS_SCORE);
        timeLabelWidth_ = bitmapWidth(NATIVE_BITMAP_HUN_MATCH_RESULTS_TIME);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawLabelCentered(screen, NATIVE_BITMAP_BUL_MATCH_RESULTS, tableY_ + 15_ZRD);
        drawLabel(screen, NATIVE_BITMAP_BUL_MATCH_RESULTS_SCORE, (int)tableX_ + 102_ZRD, screenPosY() + tableY_ + 55_ZRD);
        drawLabel(screen, NATIVE_BITMAP_BUL_MATCH_RESULTS_TIME, (int)tableX_ + 252_ZRD, screenPosY() + tableY_ + 55_ZRD);
        scoreLabelWidth_ = bitmapWidth(NATIVE_BITMAP_BUL_MATCH_RESULTS_SCORE);
        timeLabelWidth_ = bitmapWidth(NATIVE_BITMAP_BUL_MATCH_RESULTS_TIME);
    }
}

void MatchResults::initAnimation() noexcept {
    if(teamDatas_.size() &&
    Algorithms::anyOf(Application::activeMenu(), MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING, MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING) &&
    !Application::tileset()->viewOnly()) {
        bool gold = teamDatas_[0].teamId == static_cast<uint8_t>(Application::avatar());
        bool silver = teamDatas_.size() > 1 && teamDatas_[1].teamId == static_cast<uint8_t>(Application::avatar());
        bool bronze = teamDatas_.size() > 2 && teamDatas_[2].teamId == static_cast<uint8_t>(Application::avatar());

        if(!fireworksStarted_ && (gold || silver || bronze)) {
            Application::showWidget(new Fireworks(0, 0, GameObjectInterface::Type::GOT_FIREWORK_LONG_BLUE));
            Application::showWidget(new Fireworks(0, 0, GameObjectInterface::Type::GOT_FIREWORK_LONG_ORANGE));
            if(gold) {
                Application::showWidget(new Trophy(0, 0, GameObjectInterface::Type::GOT_TROPHY_NR1));
            } else if(silver) {
                Application::showWidget(new Trophy(0, 0, GameObjectInterface::Type::GOT_TROPHY_NR2));
            } else if(bronze) {
                Application::showWidget(new Trophy(0, 0, GameObjectInterface::Type::GOT_TROPHY_NR3));
            }
            fireworksStarted_ = 1;
            debug("MatchResults::initAnimation firework");
        }

    }
}

// SerializeInterface
bool MatchResults::addData(Serialize* serialize) {
    uint32_t scores  = (((((((uint32_t)redScore_) << 8) | grayScore_) << 8) | yellowScore_) << 8) | greenScore_;
    uint64_t times = (((((((uint64_t)redElapsedTime_) << 16) | grayElapsedTime_) << 16) | yellowElapsedTime_) << 16) | greenElapsedTime_;
    return BlueTriangle::addData(serialize) &&
           serialize->add(teamNum_) != Serialize::SERIALIZE_ERROR &&
           serialize->add(scores) != Serialize::SERIALIZE_ERROR &&
           serialize->add(times) != Serialize::SERIALIZE_ERROR &&
           serialize->add(looserTeam_) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* MatchResults::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    src = BlueTriangle::buildFrom(messageData, src);
    src = Memory::copy(messageData->teamCount, src);
    src = Memory::copy(messageData->scores, src);
    src = Memory::copy(messageData->elapsedTimes, src);
    src = Memory::copy(messageData->looserTeam, src);
    return src;
}

void MatchResults::setScores(uint8_t red, uint8_t gray, uint8_t yellow, uint8_t green) noexcept {
    redScore_ = red;
    grayScore_ = gray;
    yellowScore_ = yellow;
    greenScore_ = green;
    updateTeamDatas();
}

void MatchResults::setElapsedGameTimes(uint16_t red, uint16_t gray, uint16_t yellow, uint16_t green) noexcept {
    redElapsedTime_ = red;
    grayElapsedTime_ = gray;
    yellowElapsedTime_ = yellow;
    greenElapsedTime_ = green;
    updateTeamDatas();
}

uint8_t MatchResults::winnerTeam() noexcept {
    if(!teamDatas_.size()) return 255;
    return teamDatas_[0].teamId;
}

void MatchResults::setTeamNum(uint8_t teamNum) noexcept {
    teamNum_ = teamNum;
    updateTeamDatas();
}

void MatchResults::setElapsedTimes(uint64_t elapsedTimes) noexcept {
    greenElapsedTime_ = elapsedTimes & 0xffff;
    yellowElapsedTime_ = (elapsedTimes >> 16) & 0xffff;
    grayElapsedTime_ = (elapsedTimes >> 32) & 0xffff;
    redElapsedTime_ = elapsedTimes >> 48;
    updateTeamDatas();
}

void MatchResults::setScores(uint32_t scores) noexcept {
    greenScore_ = scores & 0xff;
    yellowScore_ = (scores >> 8) & 0xff;
    grayScore_ = (scores >> 16) & 0xff;
    redScore_ = scores >> 24;
    updateTeamDatas();
}
