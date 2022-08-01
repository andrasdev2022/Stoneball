#include "WorldTouchEvent.h"
#include "ObserverTouchEventSubject.h"
#include "Application.h"
#include "MediaData.h"
#include "Environment.h"
#include "Tileset.h"
#include "HeroWalkStrategy.h"
#include "Algorithms.h"
#include "Collision.h"
#include "MultiplayerWorld.h"
#include "BitmapIds.h"
#include "PlayWithFriend1.h"
#include "DeckOfCards.h"
#include "CardPull.h"
#include "CardTeleport.h"
#include "HeroStraightWalkStrategy.h"
#include "Questionmark.h"
#include "Screen.h"

bool WorldTouchEvent::notify(ObserverTouchEventSubject* subject) noexcept {
    // onTouchEvent(subject->action(), subject->x(), subject->y())

    if(subject->action() == GameObject::Event::ACTION_DOWN) {
        grabbed_ = GRABBED;
        if(grabbedX_ == 0 && grabbedY_ == 0) {
            grabbedX_ = subject->x();
            grabbedY_ = subject->y();
        } else {
            // grabbedX_ - subject->x() = A
            // GRABBEDX_ - SUBJECT->X() = A
            // GRABBEDX_ = A + SUBJECT->X()
            grabbedX_ = Application::tileset()->deltaX() + subject->x();
            grabbedY_ = Application::tileset()->deltaY() + subject->y();
            deltaX_ = Application::tileset()->deltaX();
            deltaY_ = Application::tileset()->deltaY();
        }
    } else if(subject->action() == GameObject::Event::ACTION_MOVE) {
        int moveX = abs(grabbedX_ - Application::tileset()->deltaX() - subject->x());
        int moveY = abs(grabbedY_ - Application::tileset()->deltaY() - subject->y());
        if(grabbed_ && (grabbed_ == GRABBED_AND_MOVING || moveX > 15_ZRD || moveY > 15_ZRD)) {
            grabbed_ = GRABBED_AND_MOVING;
            //std::cout  << "moveX: " << moveX<< ", moveY: " << moveY << std::endl;
            const int maxDiff = 64_ZRD;
            int diffX = grabbedX_ - subject->x();
            int diffY = grabbedY_ - subject->y();

            if(diffX > maxDiff) {diffX = maxDiff;}
            else if(diffX < -maxDiff) {diffX = -maxDiff;}

            if(diffY > maxDiff) {diffY = maxDiff;}
            else if(diffY < -maxDiff) {diffY = -maxDiff;}

            Application::tileset()->move(diffX, diffY);
            return true;
        }
    } else {
        if(subject->action() == GameObject::Event::ACTION_UP) {
            if(grabbed_ && (deltaX_ != Application::tileset()->deltaX() || deltaY_ != Application::tileset()->deltaY())) {
                grabbed_ = NOT_GRABBED;
                return true;
            }
        }
        grabbed_ = NOT_GRABBED;
    }

    if(world_) {
        if(!Application::game() || !Application::game()->worldListSize()) return false;
        setWorldPtr(Application::latestWorld());
    }
    if(subject->action() == GameObject::Event::ACTION_MOVE || !world_) {
        return false;
    }

    if(Application::isEmotesBubbleVisible()) {
        debug("WorldTouchEvent::notify");
        Application::hideEmotesBubble();
        return false;
    }

    //if(world_ && (world_->isHeroDead() || world_->areEnemiesDead()) && Application::now() - world_->timeOfDeath() > 3000) {
    if((world_->isHeroDead() || world_->areEnemiesDead()) && subject->action() == GameObject::ACTION_UP) {
    //subject->x() >= 129_ZRW && subject->y() >= 106_ZRH && subject->x() < 273_ZRW && subject->y() < 137_ZRH ) {
        int minX = (screenWidth() - bitmapWidth(NATIVE_BITMAP_LEVEL_COMPLETED_ENG)) / 2 + 133_ZRD;
        int minY = (screenHeight() - bitmapHeight(NATIVE_BITMAP_LEVEL_COMPLETED_ENG)) / 2 + 61_ZRD;
        int maxX = (screenWidth() - bitmapWidth(NATIVE_BITMAP_LEVEL_COMPLETED_ENG)) / 2 + 166_ZRD;
        int maxY = (screenHeight() - bitmapHeight(NATIVE_BITMAP_LEVEL_COMPLETED_ENG)) / 2 + 99_ZRD;
        if(subject->x() >= minX && subject->y() >= minY && subject->x() < maxX && subject->y() < maxY) {
            world_->setDefeat();
            //std::cout << "WorldTouchEvent::notify setDefeat\n";
            return true;
        }
    }

    Tileset* tileset = Application::tileset();
    if(subject->action() == GameObject::ACTION_UP && tileset && tileset->winnerTeam() == Tileset::WINNER_UNKNOWN) {
        // test the decks of cards (outside of playfield, always on top)
        if(forwardTouchEventForDeckOfCards(subject)) {
            return true;
        }

        // above the field, always on top
        if(forwardTouchEventForCards(subject)) {
            return true;
        }

        if(Application::tileset()->tilesetCardHandler()) {
            if(!world_->multiplayer()) {
                Application::tileset()->tilesetCardHandler()->hideAllCards();
            } else if(Application::tileset()->tilesetCardHandler()->anyCardVisible()) {
                // hide all cards user command
                world_->multiplayer()->onHideAllCards();
                return true;
            }
        }

        int tileX = tileset->convertWindowX2TileX(subject->x() + screenPosX());
        int tileY = tileset->convertWindowY2TileY(subject->y() + screenPosY());
        wchar_t tileContent = tileset->tileContent(tileX, tileY);

        // this condition is ALWAYS FALSE on multiplayer client side (tile content is not synched on multiplayer client) thus it must be checked
        // on server side too
        if(Algorithms::anyOf(tileContent, Tileset::invalidField,
                              Tileset::wall, Tileset::hole, Tileset::holeRed, Tileset::holeBlue,
                              Tileset::cactus1, Tileset::cactus2, Tileset::palmTree1, Tileset::palmTree2, Tileset::lake, Tileset::boat,
                              Tileset::staticStone, Tileset::building)) {
            // There is something on the tile in Single Player? Let's pop a questionmark
            popQuestionmarksAboveKnights();
            return false;
        }

        if(handleClickOnBalls(subject)) return true;

        // this condition is ALWAYS FALSE on multiplayer client side
        if(Algorithms::anyOf(tileContent, Tileset::ball, Tileset::ballRed, Tileset::ballBlue)) {
            if(handleClickOnBalls(tileContent, tileX, tileY))
                return true;
            // Something wrong with the ball in Single Player? Let's pop a questionmark
            popQuestionmarksAboveKnights();
            return false;
        }

        // do we want to select another knight?
        if(handleClickOnKnight(subject)) return true;

        // if none of the heros was clicked, the active has to move
        // the multiplayer version has to handle the situation when a client
        // command has been received
        if(!world_->multiplayer()) {
            int activeKnightId = Application::environment()->activeKnightId();
            auto heroIt = world_->knightList().find(activeKnightId);

            if(heroIt != world_->knightList().end() && tileset->walkHistoryRuleCheck()) {
                if(heroIt->second->isRunning()) {
                    if(heroIt->second->strategy()) {
                        heroIt->second->strategy()->finishStrategy();
                    }
                } else {
                    if(tileset->isTileOccupiedByAKnight(tileX, tileY)) {
                        // we cannot teleport or walk to an occupied tile
                        popQuestionmarksAboveKnights();
                        return true;
                    }

                    // the knights and the stones must not be in walking status
                    if(Application::isUndoCompatible()) {
                        tileset->updateHistory(heroIt->second.get(), MoveHistory::EventType::UNDO_POINT, VectorD());
                    }
                    auto tch = tileset->tilesetCardHandler();
                    if(tch && tch->isTeleportCardActive() && tch->getActiveTeleportCard() && tch->getActiveTeleportCard()->amount() > 0) {
                        teleport(heroIt->second.get(), tileX, tileY);
                    } else {
                        heroIt->second->setStrategy(new HeroWalkStrategy(tileX, tileY));
                    }
                }
            } else {
                // Active knight is not found in Single Player or his history is wrong? Let's pop a questionmark
                popQuestionmarksAboveKnights();
            }
            return true;
        } else {
            // Let's send a message to the server
            world_->multiplayer()->onTileClicked(tileX, tileY);
        }
    }

    return false;
}

bool WorldTouchEvent::forwardTouchEventForDeckOfCards(ObserverTouchEventSubject* subject) noexcept {
    auto tilesetCardHandler = Application::tileset()->tilesetCardHandler();
    if(!tilesetCardHandler) return false;

    if(world_->multiplayer()) {
        auto it = world_->otherElementList().find(tilesetCardHandler->deckId(Application::tileset()->activeTeamId()));
        if(it == world_->otherElementList().end()) return false;
        // Let's send a message to the server
        if(it->second->deckOfCards()->isTouchValid(subject->action(), subject->x(), subject->y())) {
            world_->multiplayer()->onDeckClicked(Application::tileset()->activeTeamId());
            // we don't want other user message after this one
            return true;
        }
    } else {
        std::map<uint32_t, BlueTriangleUPtr>::const_iterator it;
        if(Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
            it = world_->otherElementList().find(tilesetCardHandler->deckId(Application::tileset()->activeTeamId()));
        } else { // LEVELS
            it = world_->otherElementList().find(tilesetCardHandler->deckId(0));
        }
        if(it == world_->otherElementList().end()) return false;
        if(it->second->deckOfCards()->onTouchEvent(subject->action(), subject->x(), subject->y())) {
            return true;
        }
    }

    return false;
}

bool WorldTouchEvent::forwardTouchEventForCards(ObserverTouchEventSubject* subject) noexcept {
    if((world_->multiplayer() || Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) &&
    Application::tileset()->activeTeamId() == Tileset::WINNER_UNKNOWN) {
        return false;
    }
    //debug("WorldTouchEvent::forwardTouchEventForCards");
    for(auto& element : world_->otherElementList()) {
        if(!element.second->card() || !element.second->card()->visible()) continue;
        if(!world_->multiplayer()) {
            if(element.second->card()->onTouchEvent(subject->action(), subject->x(), subject->y())) {
                return true;
            }
        } else {
            // Let's send a message to the server
            //Rect rect(element.second->x(), element.second->y(), element.second->width(), element.second->height());
            //if(Collision::insideRect(subject->x() + screenPosX(), subject->y() + screenPosY(), rect)) {
            if(element.second->card()->isTouchValid(subject->action(), subject->x(), subject->y())) {
                world_->multiplayer()->onCardClicked(element.second->id());
            }
        }
    }
    return false;
}

bool WorldTouchEvent::handleClickOnKnight(ObserverTouchEventSubject* subject) noexcept {
    Tileset* tileset = Application::tileset();
    // the selected hero should get up
    for(auto& heroPair : world_->knightList()) {
        auto& hero = heroPair.second;
        Rect contentRect = hero->frameData()->contentRect();
        contentRect.x += hero->x();
        contentRect.y += hero->y();
        //std::cout << contentRect << "\n";
        //std::cout << subject->x() + screenPosX() <<", " << subject->y() + screenPosY() << "\n";
        if(!Collision::insideRect(subject->x() + screenPosX(), subject->y() + screenPosY(), contentRect)) continue;

        // click on knight must be ignored in LEVELS games
        if(!world_->multiplayer() && Application::activeMenu() != MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
            return true;
        }

        if(Application::playMode() != Application::PlayMode::TURNBASED) continue;


        // Knight must not be selected in turned based play mode, if any hero moved in this turn
        // The hero must keep the focus after he moved, we must not move with another one in the same turn
        if(!tileset->walkHistoryRuleCheck()) {
            // Player want's to change the focus in Single Player after he pushed a ball? Let's pop a questionmark
            popQuestionmarksAboveKnights();
            return true;
        }

        //  There should be an active Knight
        if(Application::environment()->activeKnightId() != std::numeric_limits<uint32_t>::max() && !tileset->isKnightInActiveTeam(hero.get())) {
            return true;
        }

        if(tileset->isThereActiveTeam() && !tileset->isKnightInActiveTeam(hero.get())) return true;

        if(!tileset->isThereActiveTeam()) {
            tileset->setActiveTeam(hero->type());
        }

        // hero is selected
        if(world_->multiplayer()) {
            world_->multiplayer()->onActivateKnight(hero->id());
            //debug("WorldTouchEvent::notify world_->multiplayer()->onActivateKnight %u", hero->id());
        } else {
            //debug("WorldTouchEvent::notify dbg4");
            // we have to ignore this click if there is a running knight
            for(auto it = world_->knightList().begin(); it != world_->knightList().end(); ++it) {
                if(it->second->isRunning()) return true;
            }
            Application::environment()->setActiveKnightId(hero->id());
            hero->getUp();
            //debug("Hero got up");
            // the rest of the heros should sit down
            nonActiveKnightsSitDown();
        }
        return true;
    }
    return false;
}

void WorldTouchEvent::nonActiveKnightsSitDown() noexcept {
    for(auto& heroPair : world_->knightList()) {
        auto& hero = heroPair.second;
        if(hero->id() != Application::environment()->activeKnightId()) {
            hero->sitDown();
            //LOGI("WorldTouchEvent::notify dbg5");
        }
    }
}

bool WorldTouchEvent::handleClickOnBalls(wchar_t tileContent, int tileX, int tileY) noexcept {
    if(Application::environment()->activeKnightId() == Environment::NONE_ACTIVE_KNIGHT) return false;
    auto tilesetCardHandler = Application::tileset()->tilesetCardHandler();
    if(!tilesetCardHandler) return false;

    Tileset* tileset = Application::tileset();
    if((world_->multiplayer() || Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) &&
    tileset->activeTeamId() >= tileset->teamCount()) {
        return true;
    }

    auto it = world_->knightList().find(Application::environment()->activeKnightId());
    if(it == world_->knightList().end()) {
        return false;
    }
    int knightTileX = tileset->convertKnightX2TileX(it->second->x());
    int knightTileY = tileset->convertKnightY2TileY(it->second->y());

    if((tileX == knightTileX && abs(tileY - knightTileY) == 1) || (tileY == knightTileY && abs(tileX - knightTileX) == 1)) {
        int targetTileX = knightTileX + (knightTileX - tileX);
        int targetTileY = knightTileY + (knightTileY - tileY);
        if(tilesetCardHandler->isPullCardActive() && tilesetCardHandler->getActivePullCard() && tilesetCardHandler->getActivePullCard()->amount() > 0 && testTargetTile(targetTileX, targetTileY)) {
            if(!tileset->walkHistoryRuleCheck()) {
                // we already pushed a ball
                popQuestionmarksAboveKnights();
                return true;
            }

            //debug("WorldTouchEvent::handleClickOnBalls UNDO_POINT before pull card");
            if(Application::isUndoCompatible()) {
                Application::tileset()->updateHistory(it->second.get(), MoveHistory::EventType::UNDO_POINT, VectorD());
            }

            if(tileContent == Tileset::ballRed) {
                debug("red ball marked for pulling");
            } else if(tileContent == Tileset::ball) {
                debug("gray ball marked for pulling");
            } else if(tileContent == Tileset::ballBlue) {
                debug("blue ball marked for pulling");
            }
            auto hs = new HeroStraightWalkStrategy(tileset->convertTileX2KnightX(targetTileX), tileset->convertTileY2KnightY(targetTileY));
            if(world_->multiplayer() || Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
                hs->addOption(HeroStraightWalkStrategy::Option::SIT_DOWN_AT_THE_END);
                hs->addOption(HeroStraightWalkStrategy::Option::ACTIVATE_NEXT_TURN);
            } else {
                hs->addOption(HeroStraightWalkStrategy::Option::STOP_AT_THE_END);
            }
            it->second->setStrategy(hs);
            // end of turn, it is useless
            //tileset->updateHistory(it->second->get(), MoveHistory::EventType::PUSH, VectorD{directionX, directionY});
            uint32_t rollingStoneId = tileset->gameObjectId(tileX, tileY);
            auto rIt = world_->rollingStoneList().find(rollingStoneId);
            if(rIt != world_->rollingStoneList().end()) {
                double directionX = targetTileX - knightTileX;
                double directionY = targetTileY - knightTileY;
                double velocity = GameObjectDescriptor::find(GameObjectInterface::Type::GOT_KNIGHT_RED, State::Type::ST_RUN).velocity();
                //debug("directionX: %f, directionY: %f, velocity: %f", directionX, directionY, velocity);
                rIt->second->push(directionX * tileset->tileWidth(), directionY * tileset->tileHeight(), velocity / 3.);
            } else {
                debug("ERROR: WorldTouchEvent::handleClickOnBalls");
            }

            auto activePullCard = tilesetCardHandler->getActivePullCard();
            activePullCard->resetCoolDown(); // decreases the amount
            if(Application::activeMenu() != MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
                activePullCard->setActive(false);
            }

            return true;
        }
    }
    return false;
}

bool WorldTouchEvent::testTargetTile(int tileX, int tileY) noexcept {
    Tileset* tileset = Application::tileset();
    auto content = tileset->tileContent(tileX, tileY);
    if(content == Tileset::emptyField) {
        for(auto& heroPair : world_->knightList()) {
            auto& hero = heroPair.second;
            if(tileset->convertKnightX2TileX(hero->x()) == tileX && tileset->convertKnightY2TileY(hero->y()) == tileY) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool WorldTouchEvent::handleClickOnBalls(ObserverTouchEventSubject* subject) noexcept {
    for(auto& stonePair : world_->rollingStoneList()) {
        auto& stone = stonePair.second;
        Rect contentRect = stone->frameData()->contentRect();
        contentRect.x += stone->x();
        contentRect.y += stone->y();
        if(!Collision::insideRect(subject->x() + screenPosX(), subject->y() + screenPosY(), contentRect)) continue;
        if(world_->multiplayer()) {
            world_->multiplayer()->onBallToPullClicked(stone->id());
            return true;
        }
        break;
    }
    return false;
}

void WorldTouchEvent::teleport(Knight* knight, int tileX, int tileY) noexcept {
    knight->setX(Application::tileset()->convertTileX2KnightX(tileX));
    knight->setY(Application::tileset()->convertTileY2KnightY(tileY));
    auto activeTeleportCard = Application::tileset()->tilesetCardHandler()->getActiveTeleportCard();
    activeTeleportCard->resetCoolDown();
    if(Application::activeMenu() != MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
        activeTeleportCard->setActive(false);
    }
    if(world_->multiplayer() || Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
        knight->sitDown();
        Application::environment()->setActiveKnightId(knight->id());
        Application::nextTurn();
    }
}

void WorldTouchEvent::popQuestionmarksAboveKnights() noexcept {
#ifndef __SERVERAPP__
    if(world_->multiplayer()) {return;}
#endif

    //debug("WorldTouchEvent::popQuestionmarksAboveKnights");

    if(Application::environment()->activeKnightId() == std::numeric_limits<uint32_t>::max()) {
        for(auto& heroPair : world_->knightList()) {
            world_->addOtherElement(BlueTriangleUPtr(new Questionmark(100., 1500., heroPair.second->id())));
        }
    } else {
       auto it = world_->knightList().find(Application::environment()->activeKnightId());
       if(it != world_->knightList().end()) {
           world_->addOtherElement(BlueTriangleUPtr(new Questionmark(100., 1500., it->second->id())));
       }
    }
}
