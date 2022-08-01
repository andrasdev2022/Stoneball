#include "MultiplayerWorld.h"
#include "stoneball.h"
#include "Knight.h"
#include "TouchEventHandler.h"
#include "Button.h"
#include "Collision.h"
#include "StaticObjects.h"
#include "MediaData.h"
#include "Vector.h"
#include "Screen.h"
#include "GameMap.h"
#include "Bitmap565.h"
#include "WorldTouchEvent.h"
#include "Application.h"
#include "UDPUserCommands.h"
#include "KnightFactory.h"
#include <exception>
#include "SoundEffect.h"
#include "Memory.h"
#include "SnapshotSyncer.h"
#include "GameObjectCollider.h"
#include "ClientTimeSyncer.h"
#include "Environment.h"
#include "UDPMessageFactory.h"
#include <functional>
#include "HeroWalkStrategy.h"
#include "Messagebox1.h"
#include "BouncingStones.h"
#include "BlueTriangleFactory.h"
#include "BitmapIds.h"
#include "Algorithms.h"
//#include "EntityFactory.h"
//#include "Attributes.h"
//#include "DrawEntityInterface.h"
#include "PlayWithFriend1.h"
#include "DeckOfCards.h"
#include "Card.h"
#include "Questionmark.h"
#include "Multiplayer5.h"
#include "Multiplayer3.h"
#include "Multiplayer1.h"
#include "EmoteFigure.h"

using namespace Bitmap565;
using namespace Application;

#include <stdlib.h>
#include <list>

namespace {
} // namespace

void MultiplayerWorld::init() noexcept
{
    srand(time(0));
    status_ = Status::ST_ONGOING_GAME;
}
/*
void MultiplayerWorld::initEntity(const UDPMessageData& msgData) noexcept {
    auto entity = EntityFactory::createEntity();
    entity->setId(msgData.id);
    //updateEntity(entity, msgData);
    addEntity(std::move(entity));
}

void MultiplayerWorld::updateEntity([[maybe_unused]]EntityInterfaceUPtr& entity, [[maybe_unused]]const UDPMessageData& msgData) noexcept {
    // set type
    entity->setAttribute(Attribute::POSITION_X, ZRD(msgData.x));
    entity->setAttribute(Attribute::FRAME_INDEX, msgData.frameIndex);
    if(msgData.skinMin != 0xffff) {
        entity->setAttribute(Attribute::SKIN_MIN, msgData.skinMin);
        entity->setAttribute(Attribute::SKIN_MAX, msgData.skinMax);
    }
    entity->setAttribute(Attribute::FLIPPED, msgData.flipped);
    entity->setAttribute(Attribute::HITPOINTS, msgData.hitPoints);
    entity->setAttribute(Attribute::MAXHITPOINTS, msgData.maxHitPoints);

    //const MediaData* frameData = gameObject->frameData();
    const MediaData* frameData = bitmapData(msgData.frameIndex, msgData.flipped);
    const Rect& contentRect = frameData->contentRect();
    double y = ZRD(msgData.contentBottom) - contentRect.y - contentRect.height;
    entity->setAttribute(Attribute::POSITION_Y, y);
}
*/

MultiplayerWorld::MultiplayerWorld(const MultiplayerWorld& w) noexcept
 : status_(w.status_)
 , timeOfDeath_(w.timeOfDeath_)
 , clientId_(w.clientId_)
 , tick_(w.tick_)
 , snapshotTimeStamp_(w.snapshotTimeStamp_)
 , keepAlive_(w.keepAlive_)
 , waitingForOpponentsMeesageboxId_(w.waitingForOpponentsMeesageboxId_)
 {
    copyEntities(w.knights_, knights_);
    copyEntities(w.particles_, particles_);
    copyEntities(w.rollingStones_, rollingStones_);
    copyEntities(w.rpgBgElements_, rpgBgElements_);
    copyEntitiesV2(w.otherElements_, otherElements_);
    //copyEntitiesV2(w.entities_, entities_);
}

World* MultiplayerWorld::copy() const noexcept {
    return new MultiplayerWorld(*this);
}

#ifdef __SERVERAPP__
void MultiplayerWorld::opponentCheck() noexcept {
    if((int)Application::playerCount() < Application::tileset()->teamCount() &&
    (Application::tileset()->winnerTeam() == Tileset::WINNER_UNKNOWN || Application::tileset()->winnerTeam() == Tileset::MATCH_CANCELLED)) {
        if(Application::tileset()->playersJoined()) {
            if((int)Application::playerCount() == 1) {
                //Application::tileset()->announceWinner(-1);
            } else if(Application::tileset()->playWithSomeone()) {// Ha az aktív játékos ez a játékos, amelyik elhagyta a játékot
                auto& quitters = Application::tileset()->playWithSomeone()->quitters();
                if(std::find(quitters.begin(), quitters.end(), Application::tileset()->activeTeamId()) != quitters.end()) {
                    Application::tileset()->nextTurn();
                }
            }
        }
        else if(!waitingForOpponentsMeesageboxId_) {
            TranslateInterface::Key message = TranslateInterface::Key::TRA__WAITING_FOR_OPPONENT;
            Messagebox1UPtr messagebox(new Messagebox1(6e4, message));
            messagebox->setShowCountDown(true);
            waitingForOpponentsMeesageboxId_ = messagebox->id();
            addOtherElement(std::move(messagebox));
        } else {
            auto it = otherElements_.find(waitingForOpponentsMeesageboxId_);
            if(it == otherElements_.end()) {
                static bool didThis = false;
                if(!didThis) {
                    didThis = true;
                    // Probably the messagebox died due to timeout
                    // The game did not start at all
                    // Raise a messagebox and stop the game/serverapp
                    Application::stopServerAt(Application::now() + 5e3);
                    for(auto& w : otherElements_) {
                        if(w.second->messagebox1()) {
                            w.second->die();
                        }
                    }
                    TranslateInterface::Key message = TranslateInterface::Key::TRA__NO_OPPONNENTS_SHOWED_UP;
                    Messagebox1UPtr messagebox(new Messagebox1(6e8, message));
                    addOtherElement(std::move(messagebox));
                    Application::tileset()->setWinnerTeam(Tileset::MATCH_CANCELLED);
                    debug("GAME OVER");
                }
            }
        }
    } else if(waitingForOpponentsMeesageboxId_){
        auto it = otherElements_.find(waitingForOpponentsMeesageboxId_);
        if(it != otherElements_.end()) {
            it->second->die();
        }
        waitingForOpponentsMeesageboxId_ = 0;
    }
}
#endif // __SERVERAPP__

// Called by SERVER and CLIENT too
void MultiplayerWorld::update([[maybe_unused]]double now) noexcept {
#ifdef __SERVERAPP__
    snapshotTimeStamp_ = now;
    opponentCheck();
    std::for_each(knights_.begin(), knights_.end(), [&](std::pair<const uint32_t, KnightUPtr>& p){
        double posX = p.second->x();
        double posY = p.second->y();
        p.second->update(now);
        GameObjectCollider collider(this);
        if(collider.testHeroStoneCollisions(p.second) || collider.testHeroRPGBgElementCollisions(p.second)) {
            // hero's move must be reverted
            p.second->setX(posX);
            p.second->setY(posY);
            return;
        }
    });
    updateEntities(particles_, now);
    updateEntities(rollingStones_, now);
    updateEntities(rpgBgElements_, now);
    updateEntities(otherElements_, now);
    //updateEntities2(entities_, now);

    // the update is called by server only
    Application::tileset()->update(now);
    Application::tileset()->tilesetCardHandler()->updateDecks(Application::now());
#else
    // these stuff should be done on the interpolated world in interpolateScreen()

    environment()->tileset()->followTheKnight();
    setScreenPos(screen()->x(), screen()->y());
    Application::tileset()->updateTexts(now); // screenPosX, screenPosY is used in updateTexts
    Application::tileset()->updateElapsedTime(now);

    bool someoneIsRunning = false;
    std::for_each(knights_.begin(), knights_.end(), [&](std::pair<const uint32_t, KnightUPtr>& p){
        someoneIsRunning |= (p.second->state()->stateType() == State::Type::ST_RUN);
    });

    if(someoneIsRunning && tileset()->winnerTeam() == Tileset::WINNER_UNKNOWN) {
        SoundEffect::play(SoundEffect::Name::PLAY_RUN_ON_SAND);
    } else if(SoundEffect::isEmpty()) {
        SoundEffect::play(SoundEffect::Name::STOP_PLAYING_RUN_ON_SAND);
    }
    sendKeepAlive();
#endif
}

void MultiplayerWorld::changeState() noexcept {
    changeEntitiesState(knights_);
    //changeEntitiesState2(entities_);
}

void MultiplayerWorld::draw([[maybe_unused]]AndroidBitmapInfo* info, void* pixels) noexcept {
    screen()->setContentPtr((uint16_t*)pixels);
    screen()->copyPreparedBackground(environment()->gameMap());
    draw2DRPG();
}

// Only client must use it
void MultiplayerWorld::sendKeepAlive() noexcept {
    double now = now_ms();
    // if there was a time synchronization
    // keep alive message in every 3rd second
    if(keepAlive_ > now || keepAlive_ + 3.e3 < now) {
        keepAlive_ = now;
        UDP udp;
        auto msg = UDPMessageFactory::createKeepAlive();
        [[maybe_unused]]bool xx = udp.sendMessage2Server(msg);
    }
}

void MultiplayerWorld::draw2DRPG() noexcept {
    //LOGI("MultiplayerWorld::draw2DRPG: %lu", knights_.size());
    //LOGI("MultiplayerWorld::draw2DRPG: screen %d %d %d %d", screen()->x(), screen()->y(), screen()->width(), screen()->height());
    struct Less {
        bool operator()(GameObject* a, GameObject* b) const {
            return a->z() < b->z();
        }
    };
    std::multiset<GameObject*, Less> sorter;
    std::for_each(knights_.begin(), knights_.end(), [&sorter](const auto& knightData){sorter.insert(knightData.second.get());});
    std::for_each(rpgBgElements_.begin(), rpgBgElements_.end(), [&sorter](const auto& rpgBgElementData){sorter.insert(rpgBgElementData.second.get());});
    std::for_each(rollingStones_.begin(), rollingStones_.end(), [&sorter](const auto& rollingStoneData){sorter.insert(rollingStoneData.second.get());});
    std::for_each(otherElements_.begin(), otherElements_.end(), [&sorter](const auto& blueTriangleData){sorter.insert(blueTriangleData.second.get());});
    std::for_each(sorter.begin(), sorter.end(), [](const auto& p){
        p->draw(screen());
        //LOGI("MultiplayerWorld::draw2DRPG: x: %f, y: %f", p->x(), p->y());
        //LOGI("MultiplayerWorld::draw2DRPG: GameObject::Type: %d", static_cast<int>(p->type()));
    });
    //for_each(entities_.begin(), entities_.end(), [](auto& entityPair){
    //    DrawEntityInterfaceNS::draw(entityPair.second, screen());
    //});
    Application::tileset()->drawTexts();
}

void MultiplayerWorld::drawBitmap(BitmapId id, AndroidBitmapInfo* info, void* pixels) noexcept {
    MediaData* data = bitmapData(id, false);
    int gameOverWidth = bitmapWidth(id);
    int gameOverHeight = bitmapHeight(id);
    Rect rect(0, 0, gameOverWidth, gameOverHeight);
    uint16_t* dst = (uint16_t*)pixels + info->width * ((info->height - gameOverHeight) / 2)  + (info->width - gameOverWidth) / 2;

    copyIntersection(dst, rect, info->width, data->data(), rect, gameOverWidth, true);
}

bool MultiplayerWorld::onTouchEvent(int action, double x, double y) noexcept {
    Unused(0, action, x, y);
    //hero_->onTouchEvent(action, x, y);
    return false;
}

bool MultiplayerWorld::onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept {
    Unused(0, sender, buttonState);
    //LOGI("MultiplayerWorld::onButtonEvent %s", GameObject::str(sender->type()).c_str());
    // TODO: add the User Input to the User Commands. A different thread should send it to the server
#ifndef __SERVERAPP__
    UDP udp;
    //UDPUserCommands userCommands;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->addData(clientId(), static_cast<unsigned char>(sender->type()), buttonState);
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
    //debug("DBG MultiplayerWorld::onButtonEvent ButtonState %d", buttonState);
#endif

    //hero_->onButtonEvent(sender, buttonState);
    return false;
}

bool MultiplayerWorld::onButtonEventServer(uint32_t clientId, GameObject* sender, ButtonState buttonState) noexcept {
    Unused(0, clientId);
    Application::tileset()->tilesetCardHandler()->hideAllCards();
    // How do I know which team is which client?
    if(sender == Application::game()->nextTurnButton()) {
        // The end turn button should not work if the player's move histor is empty or he moved back to the start position
        // In multiplayer mode, the history content is not syncronized between client and server thus the history size check is done here, but
        // the second condition must be done on the server
        //LOGI("MultiplayerWorld::onButtonEventServer");
        Application::nextTurn();
    } else {
        auto it = knights_.find(Application::environment()->activeKnightId());
        if(it != knights_.end()) {
            it->second->onButtonEvent(sender, buttonState);
        }
    }
    return false;
}

void MultiplayerWorld::onActivateKnight(uint32_t activeKnightId) noexcept {
    // The selected hero id should be added here
    UDP udp;
    //UDPUserCommands userCommands;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->addData(clientId(), activeKnightId);
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
    //debug("DBG MultiplayerWorld::onActivateKnight");
}

void MultiplayerWorld::onActivateKnightServer(uint32_t clientId, uint32_t activeKnightId) noexcept {
    Unused(0, clientId);

    // The hero must keep the focus after he moved, we must not move with another one in the same turn
    if(!Application::tileset()->walkHistoryRuleCheck()) {
        // Player want's to change the focus in Single Player after he pushed a ball? Let's pop a questionmark
        WorldTouchEvent wte;
        wte.setWorldPtr(Application::latestWorld());
        wte.popQuestionmarksAboveKnights();
        return;
    }

    // we have to ignore this click if there is a running knight
    for(auto& knightData : knightList()) {
        if(knightData.second->isRunning()) {
            return;
        }
    }

    for(auto& knightData : knightList()) {
        if(knightData.second->id() == activeKnightId) {
            if(Application::environment()->activeKnightId() != activeKnightId) {
                Application::environment()->setActiveKnightId(activeKnightId);
                Application::tileset()->setActiveTeam(knightData.second->type());
                knightData.second->getUp();
            }
        } else {
            knightData.second->sitDown();
        }
    }
}

void MultiplayerWorld::onTileClicked(int tileX, int tileY) noexcept {
    // The selected hero id should be added here
    UDP udp;
    //UDPUserCommands userCommands;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->addData(clientId(), (unsigned char)tileX, (unsigned char)tileY);
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
}

void MultiplayerWorld::onTileClickedServer(uint32_t clientId, int tileX, int tileY) noexcept {
    // I need the connection between the active team and the clientId.
    // Only the command of the clientId of the active team is accepted.
    Unused(0, clientId);
    //Application::tileset()->hideAllCards();

    // this condition is always true on client side thus it must be checked on server side too
    if(Algorithms::anyOf(Application::tileset()->tileContent(tileX, tileY), Tileset::invalidField, Tileset::ball, Tileset::ballRed, Tileset::ballBlue,
                          Tileset::wall, Tileset::hole, Tileset::holeRed, Tileset::holeBlue,
                          Tileset::cactus1, Tileset::cactus2, Tileset::palmTree1, Tileset::palmTree2, Tileset::lake, Tileset::boat,
                          Tileset::staticStone, Tileset::building)) {
        // Illegal tile? Let's pop a questionmark
        WorldTouchEvent wte;
        wte.setWorldPtr(Application::latestWorld());
        wte.popQuestionmarksAboveKnights();
        return;
    }

    uint32_t activeKnightId = Application::environment()->activeKnightId();
    //debug("MultiplayerWorld::onTileClickedServer activeKnightId %u", activeKnightId);
    auto heroIt = knightList().find(activeKnightId);
    if(heroIt != knightList().end() && Application::tileset()->walkHistoryRuleCheck()) {
        if(heroIt->second->isRunning()) {
            if(heroIt->second->strategy()) {
                heroIt->second->strategy()->finishStrategy();
            }
        } else {
            if(Application::tileset()->isTileOccupiedByAKnight(tileX, tileY)) {
                // we cannot teleport or walk to an occupied tile
                WorldTouchEvent wte;
                wte.setWorldPtr(Application::latestWorld());
                wte.popQuestionmarksAboveKnights();
                return;
            }

            auto tch = Application::tileset()->tilesetCardHandler();
            if(tch && tch->isTeleportCardActive()) {
                WorldTouchEvent wte;
                wte.setWorldPtr(Application::latestWorld());
                wte.teleport(heroIt->second.get(), tileX, tileY);
            } else {
                heroIt->second->setStrategy(new HeroWalkStrategy(tileX, tileY));
            }
        }
    } else {
        // Something wrong with the Knight id or the walk history? Let's pop a questionmark
        WorldTouchEvent wte;
        wte.setWorldPtr(Application::latestWorld());
        wte.popQuestionmarksAboveKnights();
    }
}

void MultiplayerWorld::onDeckClicked(int deckIndex) noexcept {
    UDP udp;
    //UDPUserCommands userCommands;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->addData(clientId(), (uint8_t)deckIndex);
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
}

void MultiplayerWorld::onDeckClickedServer(uint32_t clientId, int deckIndex) noexcept {
    Unused(0, clientId);
    auto pwf = Application::tileset()->tilesetCardHandler();
    if(!pwf) return;
    auto it = otherElementList().find(pwf->deckId(deckIndex));
    if(it != otherElementList().end()) {
        if(it->second->deckOfCards()->onClicked()) {
            return;
        }
    }
}

void MultiplayerWorld::onBallToPullClicked(uint32_t ballId) {
    //debug("MultiplayerWorld::onBallToPullClicked");
    UDP udp;
    //UDPUserCommands userCommands;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->addBallData(clientId(), ballId);
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
}

void MultiplayerWorld::onBallToPullClickedServer(uint32_t clientId, uint32_t ballId) {
    Unused(0, clientId);
    //debug("MultiplayerWorld::onBallToPullClickedServer");
    auto it = rollingStones_.find(ballId);
    if(it == rollingStones_.end()) {
        // Server cannot find the selected ball? Let's pop a questionmark
        WorldTouchEvent wte;
        wte.setWorldPtr(Application::latestWorld());
        wte.popQuestionmarksAboveKnights();
        return;
    }

    int ballTileX = tileset()->convertStoneX2TileX(it->second->x());
    int ballTileY = tileset()->convertStoneY2TileY(it->second->y());
    auto tileContent = tileset()->tileContent(ballTileX, ballTileY);
    if(Algorithms::noneOf(tileContent, Tileset::ball, Tileset::ballRed, Tileset::ballBlue)) {
        // Server cannot find a ball on the tile? Let's pop a questionmark
        WorldTouchEvent wte;
        wte.setWorldPtr(Application::latestWorld());
        wte.popQuestionmarksAboveKnights();
        return;
    }

    WorldTouchEvent wte;
    wte.setWorldPtr(Application::latestWorld());
    if(!wte.handleClickOnBalls(tileContent, ballTileX, ballTileY)) {
        // Something wrong with the click on the ball? Let's pop a questionmark
        wte.popQuestionmarksAboveKnights();
    }
}

void MultiplayerWorld::onCardClicked(uint32_t id) noexcept {
    UDP udp;
    //UDPUserCommands userCommands;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->addCardId(clientId(), id);
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
}

void MultiplayerWorld::onCardClickedServer(uint32_t clientId, uint32_t  cardId) noexcept {
    Unused(0, clientId);
    //Application::tileset()->hideAllCards();
    auto pwf = Application::tileset()->tilesetCardHandler();
    if(!pwf) return;
    auto it = otherElementList().find(cardId);
    if(it != otherElementList().end() && it->second->card() && it->second->card()->onClicked()) {
        return;
    }
}

void MultiplayerWorld::onHideAllCards() noexcept {
    UDP udp;
    //UDPUserCommands userCommands;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->hideAllCardsCommand(clientId());
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
}

void MultiplayerWorld::onHideAllCardsServer(uint32_t clientId) noexcept {
    Unused(0, clientId);
    Application::tileset()->tilesetCardHandler()->hideAllCards();
}

void MultiplayerWorld::onEmoteBubbleSelected(uint32_t emoteId) noexcept {
    debug("MultiplayerWorld::onEmoteBubbleSelected emoteId: %u", emoteId);
    UDP udp;
    UDPMessageInterfaceUPtr msgIF = UDPMessageFactory::create(UDPMessageInterface::TypeId::USER_COMMANDS);
    msgIF->userCommands()->addEmoteId(clientId(), emoteId, (uint8_t)Application::avatar());
    [[maybe_unused]]bool xx = udp.sendMessage2Server(msgIF);
}

void MultiplayerWorld::onEmoteBubbleSelectedServer(uint32_t clientId, uint32_t emoteId, uint8_t avatar) noexcept {
    Unused(0, clientId);
    //debug("MultiplayerWorld::onEmoteBubbleSelectedServer clientId: %u emoteId: %u", clientId, emoteId);
    //Application::tileset()->tilesetCardHandler()->showEmote(clientId, emoteId, avatar);
    addOtherElement(BlueTriangleUPtr( new EmoteFigure(0, 2000, emoteId, avatar)));
}

void MultiplayerWorld::deleteDeadObjects() noexcept {
    // Multiplayer specialisan kezeli a dead knightot
    deleteDeadObjects2(knights_);

    deleteDeadObjects2(particles_);
    deleteDeadObjects2(rollingStones_);
    deleteDeadObjects2(rpgBgElements_);
    deleteDeadObjects2(otherElements_);
    //deleteDeadObjects3(); // for entities
}

bool MultiplayerWorld::isAnyoneDead() noexcept {
    return isAnyoneDead2(particles_) ||
           isAnyoneDead2(rollingStones_) ||
           isAnyoneDead2(rpgBgElements_) ||
           isAnyoneDead2(otherElements_);
}

void MultiplayerWorld::wakeUp(double now) noexcept {
    wakeUpEntities(knights_, now);
    wakeUpEntities(particles_, now);
    wakeUpEntities(rollingStones_, now);
    if(timeOfDeath_ > 1e-7) {
        timeOfDeath_ = now;
    }
}

// GameObject's update based on the server delta snapshot
void MultiplayerWorld::sync(UDPServerDeltaSnapshot* snapshot) noexcept(false) {
    //LOGI("MultiplayerWorld::sync(UDPServerDeltaSnapshot) 1");
    if(!clientId()) {
        setClientId(snapshot->clientId());
    }

    tick_ = snapshot->tick();
    snapshotTimeStamp_ = snapshot->timeStamp();

    auto func = std::bind(&Tileset::onGameObjectChange, Application::tileset(), std::placeholders::_1, std::placeholders::_2);
    SnapshotSyncer syncer(this);
    syncer.sync(snapshot, func);

    //debug("MultiplayerWorld::sync(UDPServerDeltaSnapshot) 2");

    // The tileset has to update/recalc the modified positions,
    // because the position on the tileset is defferent here and on the server
    //Application::tileset()->recalcEveryXY(this);
    sendKeepAlive();
}

// GameObject's update based on the server snapshot
void MultiplayerWorld::sync(UDPServerSnapshot* snapshot) noexcept(false) {
    //LOGI("MultiplayerWorld::sync(UDPServerSnapshot\n");
    if(!clientId()) {
        setClientId(snapshot->clientId());
    }
    SnapshotSyncer syncer(this);
    syncer.sync(snapshot);

    // The tileset has to update/recalc every single position,
    // because the position on the tileset is different
    Application::tileset()->recalcEveryXY(this);
    sendKeepAlive();
}

// ServerApplication calls it
// The knights will be synchronised from snapshots
/*
void MultiplayerWorld::registerClient(GameObject::Type playerType, uint32_t playerId) noexcept {
    LOGI("DBG MultiplayerWorld::createPlayer playerId: %u", playerId);
    UDPMessageData msgData;
    msgData.skinMin = 0xffff;
    msgData.skinMax = 0xffff;
    msgData.hitPoints = 4000;
    msgData.maxHitPoints = 4000;
    msgData.contentBottom = 8700._ZRH;
    msgData.x = 0._ZRW;
    msgData.id = playerId;
    msgData.flipped = false;
    switch (playerType) {
    case GameObject::Type::GOT_KNIGHT_RED:
        msgData.type = 'K';
        msgData.frameIndex = NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_2;
        //msgData.state = static_cast<unsigned char>(knight->state()->stateType());
        msgData.gameObjectType = static_cast<unsigned char>(playerType);
        LOGI("DBG MultiplayerWorld::createPlayer initHero %u", msgData.id);
        initHero(msgData);
    break;
    case GameObject::Type::GOT_KNIGHT_DARK:
        msgData.type = 'K';
        msgData.frameIndex = NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_2;
        //msgData.state = static_cast<unsigned char>(knight->state()->stateType());
        msgData.gameObjectType = static_cast<unsigned char>(playerType);
        LOGI("DBG MultiplayerWorld::createPlayer initHero %u", msgData.id);
        initHero(msgData);
    break;
    default:
    break;
    }
}

// ServerApplication calls it
bool MultiplayerWorld::isClientRegistered(GameObject::Type playerType, uint32_t playerId) noexcept {
    LOGI("DBG MultiplayerWorld::isClientRegistered playerId: %u", playerId);
    switch (playerType) {
    case GameObject::Type::GOT_KNIGHT_RED:
    case GameObject::Type::GOT_KNIGHT_DARK:
        return knights_.find(playerId) != knights_.end();
    break;
    default:
    break;
    }
    return false;
}
*/

template<typename T>
void interpolateT(const std::map<uint32_t, T>& entityMap1, const std::map<uint32_t, T>& entityMap2, double alpha) {
    std::for_each(entityMap1.begin(), entityMap1.end(), [&entityMap2, alpha](auto& zPair) {
        auto it = entityMap2.find(zPair.first);
        if(it != entityMap2.end()) {
            zPair.second->setX(zPair.second->x() + (it->second->x() - zPair.second->x()) * alpha);
            zPair.second->setY(zPair.second->y() + (it->second->y() - zPair.second->y()) * alpha);
        }
    });
}

void MultiplayerWorld::interpolate(MultiplayerWorld* world, double interpolatedTime) noexcept {
    Unused(0, world, interpolatedTime);

    // MultiplayerWorld must store the time and tick of the snapshot
    // Both worlds are in the past
    if(this == world) return;

    //debug("MultiplayerWorld::interpolate: %u, %u, %f, %f, %f,", tick_, world->tick_, snapshotTimeStamp(),
    //      interpolatedTime, world->snapshotTimeStamp());

    //debug("DBG MultiplayerWorld::interpolate: snapshotTimeStamp() %f", snapshotTimeStamp());
    //debug("DBG MultiplayerWorld::interpolate: interpolatedTime %f", interpolatedTime);
    //debug("DBG MultiplayerWorld::interpolate: world->snapshotTimeStamp() %f", world->snapshotTimeStamp());

    //double originalTimeStamp = snapshotTimeStamp_;
    if(Math::abs(world->snapshotTimeStamp() - snapshotTimeStamp()) < 1e-5) {
        // Let's try to predict
        //return;
        snapshotTimeStamp_ += ClientTimeSyncer::snapshotStep_;
        //debug("DBG MultiplayerWorld::interpolate: predict");
    }
    double alpha = (interpolatedTime - snapshotTimeStamp()) / (world->snapshotTimeStamp() - snapshotTimeStamp());
    interpolateT(knights_, world->knightList(), alpha);
    interpolateT(particles_, world->particleList(), alpha);
    //snapshotTimeStamp_ = originalTimeStamp;

    // Update local screen stuff on multiplayer client
    interpolateScreen();
}

// things must be updated on the interpolated world before drawing
void MultiplayerWorld::interpolateScreen() noexcept {
    if(Application::tileset()->multiplayer5()) {
        Application::tileset()->multiplayer5()->recalcOtherObjectsXY(this);
    } else if(Application::tileset()->multiplayer3()) {
        Application::tileset()->multiplayer3()->recalcOtherObjectsXY(this);
    } else if(Application::tileset()->multiplayer1()) {
        Application::tileset()->multiplayer1()->recalcOtherObjectsXY(this);
    }
}
