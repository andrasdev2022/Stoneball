#include "AnimatedKnightsForMenu.h"
#include "Application.h"
#include "Knight.h"
#include "MediaData.h"
#include "MenuTouchEvent.h"
#include "Selectionbox.h"
#include "EnumClassOp.h"
#include "TranslateInterface.h"
#include "TouchEventHandler.h"
#include "ObserverTouchEventSubject.h"
//#include "Size.h"
//#include "Environment.h"

constexpr int OFFSET_INIT_0 = 0;
constexpr int OFFSETS_VALUES_FILLED = 1;
constexpr int OFFSETS_SET = 2;

AnimatedKnightsForMenu::AnimatedKnightsForMenu()
{

}

void AnimatedKnightsForMenu::init(int avatarNum) noexcept {
    MenuTouchEvent* menuTouchEvent = Application::menuTouchEvent();

    if(!multiplayerAnimatedHeros_.size() && menuTouchEvent) {
        if(offsetsState_ == OFFSETS_SET) {
            offsetsState_ = OFFSETS_VALUES_FILLED;
        }
        if(avatarNum > 0) {
            multiplayerAnimatedHeros_.push_back(std::make_shared<Knight>(262_ZRD, 225_ZRD, GameObject::Type::GOT_KNIGHT_RED));
            //debug("AnimatedKnightsForMenu::init multiplayerAnimatedHeros_[%d]->x(): %f", 0, multiplayerAnimatedHeros_[0]->x());
        }
        if(avatarNum > 1) {
            multiplayerAnimatedHeros_.push_back(std::make_shared<Knight>(310_ZRD, 225_ZRD, GameObject::Type::GOT_KNIGHT_DARK));
            //debug("AnimatedKnightsForMenu::init multiplayerAnimatedHeros_[%d]->x(): %f", 1, multiplayerAnimatedHeros_[1]->x());
        }
        if(avatarNum > 2) {
            multiplayerAnimatedHeros_.push_back(std::make_shared<Knight>(358_ZRD, 225_ZRD, GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE));
        }
        if(avatarNum > 3) {
            multiplayerAnimatedHeros_.push_back(std::make_shared<Knight>(406_ZRD, 225_ZRD, GameObject::Type::GOT_KNIGHT_GREEN_FEMALE));
        }
    }
}

void AnimatedKnightsForMenu::update(double now) noexcept {
    if(offsetsState_ == OFFSETS_VALUES_FILLED) {
        offsetsState_ = OFFSETS_SET;
        //debug("xOffset: %d", xOffset_);
        for(size_t i = 0; i < multiplayerAnimatedHeros_.size(); ++i) {
            //debug("AnimatedKnightsForMenu::update multiplayerAnimatedHeros_[%d]->x(): %f", i, multiplayerAnimatedHeros_[i]->x());
            multiplayerAnimatedHeros_[i]->setX(multiplayerAnimatedHeros_[i]->x() + xOffset_);
            multiplayerAnimatedHeros_[i]->setY(multiplayerAnimatedHeros_[i]->y() + yOffset_);
        }
    }
    if(offsetsState_ != OFFSET_INIT_0 && !selectionboxForMapOffsetUpdated_ && selectionboxForMaps_) {
        selectionboxForMaps_->setX(selectionboxForMaps_->x() + xOffset_);
        selectionboxForMaps_->setY(selectionboxForMaps_->y() + yOffset_);
        selectionboxForMapOffsetUpdated_ = true;
    }
    for(size_t i = 0; i < multiplayerAnimatedHeros_.size(); ++i) {
        /*
        SizeI sizeI(0,0);
        if(Application::environment()) {
            sizeI = Application::environment()->gameMap()->size();
        } else {
            sizeI = SizeI(screenWidth(), screenHeight());
        }

        const int maxWidth = sizeI.width;
        const int maxHeight = sizeI.height;
        debug("AnimatedKnightsForMenu::update maxWidth: %d, maxHeight: %d", maxWidth, maxHeight);
        debug("AnimatedKnightsForMenu::update BEFORE multiplayerAnimatedHeros_[%d]->x(): %f", i, multiplayerAnimatedHeros_[i]->x());
        debug("AnimatedKnightsForMenu::update BEFORE frameIndex: %d", (int)multiplayerAnimatedHeros_[i]->frameIndex());
        debug("AnimatedKnightsForMenu::update BEFORE velocity: %f", multiplayerAnimatedHeros_[i]->velocity());
        debug("AnimatedKnightsForMenu::update BEFORE width: %d, height: %d", multiplayerAnimatedHeros_[i]->width(), multiplayerAnimatedHeros_[i]->height());
        */
        multiplayerAnimatedHeros_[i]->update(now);
        /*
        debug("AnimatedKnightsForMenu::update AFTER multiplayerAnimatedHeros_[%d]->x(): %f", i, multiplayerAnimatedHeros_[i]->x());
        debug("AnimatedKnightsForMenu::update AFTER frameIndex: %d", (int)multiplayerAnimatedHeros_[i]->frameIndex());
        debug("AnimatedKnightsForMenu::update AFTER velocity: %f", multiplayerAnimatedHeros_[i]->velocity());
        */
    }
}

bool AnimatedKnightsForMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(offsetsState_ != OFFSETS_SET) return false;

    for(size_t i = 0; i < multiplayerAnimatedHeros_.size(); ++i) {
        multiplayerAnimatedHeros_[i]->draw(info, pixels);
    }
    return multiplayerAnimatedHeros_.size();
}

void AnimatedKnightsForMenu::sitDown() noexcept {
    for(auto& knight : multiplayerAnimatedHeros_) {
        knight->sitDown();
    }
}

void AnimatedKnightsForMenu::clear() noexcept {
    multiplayerAnimatedHeros_.clear();
}

void AnimatedKnightsForMenu::add(KnightSPtr knight) noexcept {
     multiplayerAnimatedHeros_.push_back(knight);
}

void AnimatedKnightsForMenu::selectionboxCallbackForMaps([[maybe_unused]]Selectionbox* selectionbox, [[maybe_unused]]int selectedLineNr) noexcept {
    if(selectedLineNr == -1) return;
    multiplayerAnimatedHeros_.clear();

    lastSelectedLineNr_ = selectedLineNr;

    auto selectedItem = selectionbox->selectedItem();
    int selectedMap = std::stoi(selectedItem.itemData);
    if(selectedMap >= static_cast<int>(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_5)) {
        init(4);
    } else if(selectedMap >= static_cast<int>(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_3)) {
        init(3);
    } else {
        init(2);
    }

}

// Multiplayer Create Game and On One Device use it
void AnimatedKnightsForMenu::initSelectionboxForMaps() noexcept {
    selectionboxForMapOffsetUpdated_ = false;
    selectionboxForMaps_ = new Selectionbox(0, 0);
    selectionboxForMaps_->setX(233_ZRD);
    selectionboxForMaps_->setY(110_ZRD);
    selectionboxForMaps_->setWidth(416_ZRD - 233_ZRD);
    selectionboxForMaps_->setHeight(20_ZRD);
    //debug("AnimatedKnightsForMenu::initSelectionboxForMaps: selectionboxForMaps_->x(): %f", selectionboxForMaps_->x());

    std::wstring label;
    for(auto i = EnumClassOp(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1); i <= EnumClassOp(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_6);++i) {
        switch((LevelFactory::GameLevel)i) {
        case LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1:
            label = (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER_MAP1_LABEL);
            selectionboxForMaps_->addItem(Selectionbox::Item{label, std::to_wstring(i)});
            break;
        case LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_2:
            label = (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER_MAP2_LABEL);
            selectionboxForMaps_->addItem(Selectionbox::Item{label, std::to_wstring(i)});
            break;
        case LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_3:
            label = (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER_MAP3_LABEL);
            selectionboxForMaps_->addItem(Selectionbox::Item{label, std::to_wstring(i)});
            break;
        case LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_4:
            label = (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER_MAP4_LABEL);
            selectionboxForMaps_->addItem(Selectionbox::Item{label, std::to_wstring(i)});
            break;
        case LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_5:
            label = (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER_MAP5_LABEL);
            selectionboxForMaps_->addItem(Selectionbox::Item{label, std::to_wstring(i)});
            break;
        case LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_6:
            label = (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER_MAP6_LABEL);
            selectionboxForMaps_->addItem(Selectionbox::Item{label, std::to_wstring(i)});
            break;
        default: break;
        }
    }
    selectionboxForMaps_->setName("MAPS");
    selectionboxForMaps_->setSelectedLineNr(lastSelectedLineNr_);
    auto callback = std::bind(&AnimatedKnightsForMenu::selectionboxCallbackForMaps, this, std::placeholders::_1, std::placeholders::_2);
    selectionboxForMaps_->addSelectionCallback(callback);
    Application::touchEventHandler()->add(selectionboxForMaps_);
    Application::showWidget(selectionboxForMaps_);
}

bool AnimatedKnightsForMenu::avatarSelectionCheck(ObserverTouchEventSubject* subject) noexcept {
    bool selected = false;
    Application::setAvatar(Application::Avatar::UNDEFINED);
    for(size_t i = 0; i < multiplayerAnimatedHeros_.size(); ++i) {
        auto& knight = multiplayerAnimatedHeros_[i];
        if(subject->x() >= knight->x() && subject->x() <= knight->x() + knight->width() &&
        subject->y() >= knight->y() && subject->y() <= knight->y() + knight->height()) {
            switch(i) {
            case 0: Application::setAvatar(Application::Avatar::RED); knight->getUp(); selected = true;
                break;
            case 1: Application::setAvatar(Application::Avatar::DARK); knight->getUp(); selected = true;
                break;
            case 2: {
                int selectedMapNr = selectionboxForMaps_->selectedLineNr();
                if(selectedMapNr >= 2) { // maps with 3 teams
                    Application::setAvatar(Application::Avatar::BLONDE_FEMALE);
                    knight->getUp();
                    selected = true;
                }
                break;
            }
            case 3: {
                int selectedMapNr = selectionboxForMaps_->selectedLineNr();
                if(selectedMapNr >= 4) { // maps with 4 teams
                    Application::setAvatar(Application::Avatar::GREEN_FEMALE);
                    knight->getUp();
                    selected = true;
                }
                break;
            }
            default:
                break;
            }
        } else {
            knight->sitDown();
        }
    }
    return selected;
}

bool AnimatedKnightsForMenu::avatarSelectionCheckSimple(ObserverTouchEventSubject* subject) noexcept {
    bool selected = false;
    setAvatar(Application::Avatar::UNDEFINED);
    for(size_t i = 0; i < multiplayerAnimatedHeros_.size(); ++i) {
        auto& knight = multiplayerAnimatedHeros_[i];
        if(subject->x() >= knight->x() && subject->x() <= knight->x() + knight->width() &&
        subject->y() >= knight->y() && subject->y() <= knight->y() + knight->height())
        {
            switch(i) {
            case 0: setAvatar(Application::Avatar::RED); knight->getUp(); selected = true; break;
            case 1: setAvatar(Application::Avatar::DARK); knight->getUp(); selected = true; break;
            case 2: setAvatar(Application::Avatar::BLONDE_FEMALE); knight->getUp(); selected = true; break;
            case 3: setAvatar(Application::Avatar::GREEN_FEMALE); knight->getUp(); selected = true; break;
            default:
                break;
            }
        } else {
            knight->sitDown();
        }
    }
    return selected;
}

int AnimatedKnightsForMenu::standingAvatar() noexcept {
    for(size_t i = 0; i < multiplayerAnimatedHeros_.size(); ++i) {
        auto& knight = multiplayerAnimatedHeros_[i];
        if(knight->isIdle()) {return i;}
    }
    return static_cast<int>(Application::Avatar::UNDEFINED);
}

void AnimatedKnightsForMenu::initOffsets(int xOffset, int yOffset) noexcept {
    xOffset_ = xOffset;
    yOffset_ = yOffset;
    offsetsState_ = OFFSETS_VALUES_FILLED;
}
