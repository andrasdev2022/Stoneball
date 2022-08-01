#include "SnapshotSyncer.h"
#include "Application.h"
#include "Rect.h"
#include "MediaData.h"
#include "KnightFactory.h"
#include "MoveHistory.h"
#include "Environment.h"
#include "BouncingStones.h"
#include "Messagebox1.h"
#include "BlueTriangleFactory.h"
#include "BitmapIds.h"
#include "Algorithms.h"
#include "PlayWithFriend1.h"
#include "PlayWithFriend3.h"
#include "Card.h"
#include "SoundEffect.h"
#include "Questionmark.h"
#include "MatchResults.h"
#include "EmoteFigure.h"

namespace {

template<typename T>
T* getObjectPtr(const std::map<uint32_t, std::unique_ptr<T>>& gameObjects, uint32_t id) noexcept(false) {
    auto it = gameObjects.find(id);
    if(it != gameObjects.end()) {
        return it->second.get();
    }
    return nullptr;
}

template<typename T>
void sync2(const std::list<UDPMessageData>& messageData, std::map<uint32_t, T>& gameObjects) noexcept(false) {
    // we might miss a few packages and a gameobject died
    if(messageData.size() < gameObjects.size()) {
        std::set<uint32_t> temp;
        std::for_each(messageData.begin(), messageData.end(), [&temp](const UDPMessageData& m){
            temp.insert(m.id);
        });
        for(auto it = gameObjects.begin(); it != gameObjects.end();) {
            if(temp.find(it->second->id()) == temp.end()) {
                //LOGI("SnapshotSyncer sync2 GameObject::Type: %s", GameObject::str(it->second->type()).c_str());
                it = gameObjects.erase(it);
            } else {
                ++it;
            }
        }
    }
}

} // namespace

void SnapshotSyncer::sync(const UDPServerDeltaSnapshot* snapshot, DeltaSPostCallback d) noexcept {
    for(auto& msgData : snapshot->builtData()) {
        sync(msgData, d);
    }
    if(snapshot->isMoveHistorySizePresent()) {
        syncMoveHistory(snapshot->moveHistorySize());
    }
    if(snapshot->isActiveKnightIdPresent()) {
        syncActiveKnightId(snapshot->activeKnightId());
    }
    if(snapshot->isActiveTeamIdPresent()) {
        syncActiveTeamId(snapshot->activeTeamId());
    }
    if(snapshot->isScorePresent()) {
        syncScore(snapshot->score());
    }
    if(snapshot->isElapsedGameTime1Present()) {
        syncElapsedGameTime(0, snapshot->elapsedGameTime1());
    }
    if(snapshot->isElapsedGameTime2Present()) {
        syncElapsedGameTime(1, snapshot->elapsedGameTime2());
    }
    if(snapshot->isElapsedGameTime3Present()) {
        syncElapsedGameTime(2, snapshot->elapsedGameTime3());
    }
    if(snapshot->isElapsedGameTime4Present()) {
        syncElapsedGameTime(3, snapshot->elapsedGameTime4());
    }
    if(snapshot->isWinnerTeamPresent()) {
        syncWinnerTeam(snapshot->winnerTeam());
    }
    if(snapshot->isQuittersPresent()) {
        syncQuitters(snapshot->quitters());
    }
    if(snapshot->isTeamNamesPresent()) {
        syncTeamNames(snapshot->teamNames());
    }
    if(snapshot->isViewerCountPresent()) {
        syncViewerCount(snapshot->viewerCount());
    }
}

void SnapshotSyncer::createGameObject(const UDPServerDeltaSnapshot::MessageData& msgData) noexcept {
    switch(fullSnapshot_.type) {
    case 'K': {
        //debug("SnapshotSyncer::sync 901");
        KnightUPtr knight = KnightFactory::create(static_cast<GameObject::Type>(msgData.gameObjectType));
        knight->setId(fullSnapshot_.id);
        gameObject_ = knight.get();
        knight_ = knight.get();
        world_->knights_.emplace(knight->id(), std::move(knight));
    }
    break;
    case 'P': {
        //debug("SnapshotSyncer::sync 905");
        ParticleUPtr particle = std::make_unique<Particle>(0, 0, static_cast<GameObject::Type>(msgData.gameObjectType));
        particle->setId(fullSnapshot_.id);
        gameObject_ = particle.get();
        world_->particles_.emplace(particle->id(), std::move(particle));
    }
    break;
    case 'S': {
        //debug("SnapshotSyncer::sync 905");
        RollingStoneUPtr stone = std::make_unique<RollingStone>(0, 0, static_cast<GameObject::Type>(msgData.gameObjectType),
                                                                static_cast<RollingStone::Color>(msgData.color));
        stone->setId(fullSnapshot_.id);
        gameObject_ = stone.get();
        stone_ = stone.get();
        world_->rollingStones_.emplace(stone->id(), std::move(stone));
    }
    break;
    case 'O': {
        //debug("SnapshotSyncer::sync 905");
        if(BlueTriangleUPtr otherElement = BlueTriangleFactory::create(static_cast<GameObject::Type>(msgData.gameObjectType))) {
            otherElement->setId(fullSnapshot_.id);
            gameObject_ = otherElement.get();
            blueTriangle_ = otherElement.get();
            world_->otherElements_.emplace(otherElement->id(), std::move(otherElement));
        }
    }
    break;
    case 'R': {
        //debug("SnapshotSyncer::sync 905");
        RPGBgElementUPtr rpgBgElement = std::make_unique<RPGBgElement>(0, 0, NATIVE_BITMAP_LAKE_01);
        rpgBgElement->setId(fullSnapshot_.id);
        gameObject_ = rpgBgElement.get();
        world_->rpgBgElements_.emplace(rpgBgElement->id(), std::move(rpgBgElement));
    }
    break;
    default:
        // TODO: exception?
        debug("ERROR: Unknown field id");
        return;
        break;
    }
}

void SnapshotSyncer::sync(const UDPServerDeltaSnapshot::MessageData& msgData, DeltaSPostCallback deltaSPostCallback) noexcept {
    if(msgData.field > UDPMessageData::Fields::Field_INDICATOR_REMOVED) {
        debug("ERROR: SnapshotSyncer::sync: Illegal msgData.field value");
    }
    switch (static_cast<UDPMessageData::Fields>(msgData.field)) {
    case UDPMessageData::Fields::Field_type: // first in the list, mandatory
        //debug("SnapshotSyncer::sync msgData.type: %c", msgData.type);
        fullSnapshot_.type = msgData.type;
        gameObject_ = nullptr;
        knight_ = nullptr;
        stone_ = nullptr;
        blueTriangle_ = nullptr;
        break;
    case UDPMessageData::Fields::Field_id: // gameobjectid, second in the list, mandatory
        //debug("SnapshotSyncer::sync gameobjectid: %d", msgData.id);
        fullSnapshot_.id = msgData.id;
        switch(fullSnapshot_.type) {
        case 'K': knight_       = getObjectPtr(world_->knightList(),   msgData.id); gameObject_ = knight_; break;
        case 'P': gameObject_   = getObjectPtr(world_->particleList(), msgData.id); break;
        case 'S': stone_        = getObjectPtr(world_->rollingStoneList(), msgData.id); gameObject_ = stone_; break;
        case 'O': blueTriangle_ = getObjectPtr(world_->otherElementList(), msgData.id); gameObject_ = blueTriangle_; break;
        case 'R': gameObject_   = getObjectPtr(world_->rpgBgElementList(), msgData.id); break;
        default:
            // TODO: exception?
            debug("ERROR: Unknown field id");
            return;
            break;
        }
        //if(gameObject_) {
        //    gameObject_->setId(msgData.id);
        //}
        break;
    case UDPMessageData::Fields::Field_x:
        //debug("SnapshotSyncer::sync 3");
        if(gameObject_) {
            gameObject_->setX(ZRD(msgData.x));
            if(deltaSPostCallback) {
                deltaSPostCallback(gameObject_, UDPMessageData::Fields::Field_x);
            }
        }
        break;
    case UDPMessageData::Fields::Field_contentBottom:
        //debug("SnapshotSyncer::sync 4");
        if(gameObject_) {
            const MediaData* frameData = gameObject_->frameData();
            const Rect& contentRect = frameData->contentRect();
            double y = ZRD(msgData.contentBottom) - contentRect.y - contentRect.height;
            gameObject_->setY(y);
            if(deltaSPostCallback) {
                deltaSPostCallback(gameObject_, UDPMessageData::Fields::Field_contentBottom);
            }
        }
        break;
    case UDPMessageData::Fields::Field_frameIndex:
        //debug("SnapshotSyncer::sync 5");
        if(gameObject_) {
            gameObject_->setFrameIndex(msgData.frameIndex);
        }
        break;
    case UDPMessageData::Fields::Field_skinMin:
        //debug("SnapshotSyncer::sync 6");
        if(gameObject_) {
            gameObject_->setSkinMin(msgData.skinMin);
        }
        break;
    case UDPMessageData::Fields::Field_skinMax:
        //debug("SnapshotSyncer::sync 7");
        if(gameObject_) {
            gameObject_->setSkinMax(msgData.skinMax);
        }
        break;
    case UDPMessageData::Fields::Field_state:
        //debug("SnapshotSyncer::sync 8");
        //ptr = Memory::copy(msgData.state, ptr);
        if(knight_ && fullSnapshot_.type == 'K') {
            knight_->forceState(static_cast<State::Type>(msgData.state));
        }
        break;
    case UDPMessageData::Fields::Field_gameObjectType:
        //debug("SnapshotSyncer::sync 9");
        //ptr = Memory::copy(msgData.gameObjectType, ptr);
        if(!gameObject_) {
            if(msgData.gameObjectType >= static_cast<unsigned char>(GameObject::Type::COUNT)) {
                debug("ERROR: SnapshotSyncer::sync Illegal gameObjectType: %d", (int)msgData.gameObjectType);
            }

            // A new GameObject must be created
            createGameObject(msgData);
        }
        break;
    case UDPMessageData::Fields::Field_hitPoints:
        //debug("SnapshotSyncer::sync 10");
        if(gameObject_) {
            gameObject_->setHitPoints(msgData.hitPoints, gameObject_->maxHitPoints());
        }
        break;
    case UDPMessageData::Fields::Field_maxHitPoints:
        //debug("SnapshotSyncer::sync 11");
        if(gameObject_) {
            gameObject_->setHitPoints(gameObject_->hitPoints(), msgData.maxHitPoints);
        }
        break;
    case UDPMessageData::Fields::Field_linkedGameObjectId:
        //debug("SnapshotSyncer::sync 11");
        if(blueTriangle_ && blueTriangle_->questionmark()) {
            blueTriangle_->questionmark()->setLinkedGameObjectId(msgData.linkedGameObjectId);
        }
        break;
    case UDPMessageData::Fields::Field_dead:
        //debug("SnapshotSyncer::sync 13");
        //ptr = Memory::copy(msgData.dead, ptr);
        if(gameObject_) {
            if(msgData.dead) { // theoretically there will not be more msgData about this one
                removeGameObject();
                if(stone_) {
                    SoundEffect::play(SoundEffect::Name::PLAY_GOAL);
                }
            } else {
                //updateHero(it->second, msgData);
            }
        }
        break;
    case UDPMessageData::Fields::Field_flipped:
        //debug("SnapshotSyncer::sync 14");
        if(gameObject_) {
            gameObject_->setFlipped(msgData.flipped);
        }
        break;
    case UDPMessageData::Fields::Field_color:
        //debug("SnapshotSyncer::sync 12");
        if(stone_) {
            stone_->setColor(static_cast<RollingStone::Color>(msgData.color));
        }
        break;
    case UDPMessageData::Fields::Field_messageKey:
        //debug("SnapshotSyncer::sync 12");
        if(blueTriangle_ && blueTriangle_->messagebox1()) {
            blueTriangle_->messagebox1()->setMessage(static_cast<TranslateInterface::Key>(msgData.messageKey));
        }
        break;
    case UDPMessageData::Fields::Field_visible:
        if(blueTriangle_) {
            if(blueTriangle_->card()) {
                blueTriangle_->card()->setVisible(msgData.visible);
            } else if(blueTriangle_->questionmark()){
                blueTriangle_->questionmark()->setVisible(msgData.visible);
            } else if(blueTriangle_->emoteFigure()){
                blueTriangle_->emoteFigure()->setVisible(msgData.visible);
            }
        }
        break;
    case UDPMessageData::Fields::Field_teamIndex:
        if(blueTriangle_) {
            if(blueTriangle_->card()) {
                blueTriangle_->card()->setTeamIndex(msgData.teamIndex);
            } else if(blueTriangle_->emoteFigure()) {
                blueTriangle_->emoteFigure()->setAvatar(msgData.teamIndex);
            }
        }
        break;
    case UDPMessageData::Fields::Field_amount:
        if(blueTriangle_ && blueTriangle_->card()) {
            blueTriangle_->card()->setAmount(msgData.amount);
        }
        break;
    case UDPMessageData::Fields::Field_coolDown:
        if(blueTriangle_ && blueTriangle_->card()) {
            blueTriangle_->card()->setCoolDown(msgData.coolDown);
        }
        break;
    case UDPMessageData::Field_scores:
        if(blueTriangle_ && blueTriangle_->matchResults()) {
            blueTriangle_->matchResults()->setScores(msgData.scores);
        }
        break;
    case UDPMessageData::Field_elapsedTimes:
        if(blueTriangle_ && blueTriangle_->matchResults()) {
            blueTriangle_->matchResults()->setElapsedTimes(msgData.elapsedTimes);
        }
        break;
    case UDPMessageData::Field_teamCount:
        if(blueTriangle_ && blueTriangle_->matchResults()) {
            blueTriangle_->matchResults()->setTeamNum(msgData.teamCount);
        }
        break;
    case UDPMessageData::Field_looserTeam:
        if(blueTriangle_ && blueTriangle_->matchResults()) {
            blueTriangle_->matchResults()->setLooserTeam(msgData.looserTeam);
        }
        break;
    case UDPMessageData::Field_showCountDown:
        if(blueTriangle_ && blueTriangle_->messagebox1()) {
            blueTriangle_->messagebox1()->setShowCountDown(msgData.showCountDown);
        }
        break;
    case UDPMessageData::Field_spareTime:
        if(blueTriangle_ && blueTriangle_->messagebox1()) {
            blueTriangle_->messagebox1()->setSpareTime(msgData.spareTime);
        }
        break;
    case UDPMessageData::Field_messageString:
        if(blueTriangle_ && blueTriangle_->messagebox1()) {
            blueTriangle_->messagebox1()->setMessageString(msgData.string);
        }
        break;
    case UDPMessageData::Fields::Field_INDICATOR_REMOVED:
        //debug("SnapshotSyncer::sync 15");
        if(gameObject_) {
            // it must be removed
            removeGameObject();
        }
        break;
    }
}

void SnapshotSyncer::syncMoveHistory(uint16_t moveHistorySize) noexcept {
    Application::tileset()->clearHistory();
    if(moveHistorySize != Application::tileset()->historySize()) {
        // dummy move
        Application::tileset()->updateHistory(world_->knights_.begin()->second.get(), MoveHistory::EventType::MOVE, VectorD());
    }
}

void SnapshotSyncer::syncActiveKnightId(uint32_t activeKnightId) noexcept {
    //LOGI("SnapshotSyncer::syncActiveKnightId %u", activeKnightId);
    Application::environment()->setActiveKnightId(activeKnightId);
}

void SnapshotSyncer::syncActiveTeamId(unsigned char activeTeamId) noexcept {
    auto tileset = Application::tileset();
    auto oldActiveTeamId = tileset->activeTeamId();
    //debug("SnapshotSyncer::syncActiveTeamId: oldActiveTeamId %u, activeTeamId: %d, avatar: %d", oldActiveTeamId, activeTeamId, (int)Application::avatar());
    tileset->setActiveTeamId(activeTeamId);
    if(Algorithms::anyOf(Application::activeMenu(), MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING, MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING)) {
        if(tileset->viewOnly()) {
            Application::hideNextTurnButton();
            Application::hideArrowButtons();
        } else if(Application::gameLevel() <= LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP2) {
            if(oldActiveTeamId != 0xff && oldActiveTeamId != activeTeamId) {
                SoundEffect::play(SoundEffect::Name::PLAY_DRUMS);
            }

            if((Application::avatar() == Application::Avatar::DARK && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::DARK_TEAM)
            || (Application::avatar() == Application::Avatar::RED && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::RED_TEAM)) {
                Application::showNextTurnButton();
            } else {
                Application::hideNextTurnButton();
            }
        } else if(Application::gameLevel() <= LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP4) {
            if(oldActiveTeamId != 0xff && oldActiveTeamId != activeTeamId) {
                SoundEffect::play(SoundEffect::Name::PLAY_DRUMS);
            }
            if((Application::avatar() == Application::Avatar::DARK && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::DARK_TEAM)
            || (Application::avatar() == Application::Avatar::RED && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::RED_TEAM)
            || (Application::avatar() == Application::Avatar::BLONDE_FEMALE && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::BLONDE_FEMALE_TEAM)) {
                Application::showNextTurnButton();
            } else {
                Application::hideNextTurnButton();
            }
        } else if(Application::gameLevel() <= LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP6) {
            if(oldActiveTeamId != 0xff && oldActiveTeamId != activeTeamId) {
                SoundEffect::play(SoundEffect::Name::PLAY_DRUMS);
            }
            if((Application::avatar() == Application::Avatar::DARK && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::DARK_TEAM)
            || (Application::avatar() == Application::Avatar::RED && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::RED_TEAM)
            || (Application::avatar() == Application::Avatar::BLONDE_FEMALE && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::BLONDE_FEMALE_TEAM)
            || (Application::avatar() == Application::Avatar::GREEN_FEMALE && static_cast<PlayWithSomeone::Team>(tileset->activeTeamId()) == PlayWithSomeone::Team::GREEN_FEMALE_TEAM)) {
                Application::showNextTurnButton();
            } else {
                Application::hideNextTurnButton();
            }
        }
    }

}

void SnapshotSyncer::syncScore(uint32_t score) noexcept {
    Application::tileset()->setScore(score);
}

void SnapshotSyncer::syncElapsedGameTime(uint8_t idx, uint16_t value) noexcept {
    Application::tileset()->setElapsedGameTime(idx, value);
}

void SnapshotSyncer::syncWinnerTeam(uint8_t team) noexcept {
    Application::tileset()->setWinnerTeam(team);
}

void SnapshotSyncer::syncQuitters(const std::vector<uint8_t>& quitters) const noexcept {
    Application::tileset()->setQuitters(quitters);
}

void SnapshotSyncer::removeGameObject() noexcept {
    switch(fullSnapshot_.type) {
    case 'K': world_->knights_.erase(gameObject_->id()); break;
    case 'P': world_->particles_.erase(gameObject_->id()); break;
    case 'S': world_->rollingStones_.erase(gameObject_->id()); break;
    case 'O': world_->otherElements_.erase(gameObject_->id()); break;
    case 'R': world_->rpgBgElements_.erase(gameObject_->id()); break;
    default:
        // TODO: exception?
        debug("ERROR: Unknown field id");
        return;
        break;
    }
}

void SnapshotSyncer::syncViewOnly(uint8_t viewOnly) noexcept {
    Application::tileset()->setViewOnly(viewOnly);
}

void SnapshotSyncer::syncAvatar(uint8_t avatar) noexcept {
    //debug("SnapshotSyncer::syncAvatar %u", avatar);
    Application::setAvatar(Application::GOT2Avatar((GameObjectInterface::Type)avatar));
}

void SnapshotSyncer::syncPlayTime(uint8_t playTime) noexcept {
    Application::setPlayTime(playTime);
}

void SnapshotSyncer::syncTeamNames(const std::vector<std::wstring>& teamNames) noexcept {
    for(size_t i = 0; i < teamNames.size(); ++i) {
        Application::tileset()->setTeamName(i, teamNames.at(i));
    }
}

void SnapshotSyncer::syncViewerCount(uint16_t viewerCount) noexcept {
    Application::setViewerCount(viewerCount);
}

void SnapshotSyncer::sync(UDPServerSnapshot* snapshot) noexcept(false) {
    world_->tick_ = snapshot->tick();
    world_->snapshotTimeStamp_ = snapshot->timeStamp();

    syncAvatar(snapshot->avatar());

    sync3(snapshot->knights(), &SnapshotSyncer::initHero, &SnapshotSyncer::updateHero, world_->knights_, 'K');
    sync2(snapshot->knights(), world_->knights_);

    sync3(snapshot->particles(), &SnapshotSyncer::initParticle, &SnapshotSyncer::updateParticle, world_->particles_, 'P');
    sync2(snapshot->particles(), world_->particles_);

    sync3(snapshot->rollingStones(), &SnapshotSyncer::initRollingStone, &SnapshotSyncer::updateRollingStone, world_->rollingStones_, 'S');
    sync2(snapshot->rollingStones(), world_->rollingStones_);

    sync3(snapshot->otherElements(), &SnapshotSyncer::initOtherElement, &SnapshotSyncer::updateOtherElement, world_->otherElements_, 'O');
    sync2(snapshot->otherElements(), world_->otherElements_);

    sync3(snapshot->rpgBgElements(), &SnapshotSyncer::initRPGBgElement, &SnapshotSyncer::updateRPGBgElement, world_->rpgBgElements_, 'R');
    sync2(snapshot->rpgBgElements(), world_->rpgBgElements_);

    syncMoveHistory(snapshot->moveHistorySize());
    syncActiveKnightId(snapshot->activeKnightId());
    syncViewOnly(snapshot->viewOnly());
    syncActiveTeamId(snapshot->activeTeamId());
    syncScore(snapshot->score());
    syncElapsedGameTime(0, snapshot->elapsedGameTime1());
    syncElapsedGameTime(1, snapshot->elapsedGameTime2());
    syncElapsedGameTime(2, snapshot->elapsedGameTime3());
    syncElapsedGameTime(3, snapshot->elapsedGameTime4());
    syncPlayTime(snapshot->playTime());
    syncWinnerTeam(snapshot->winnerTeam());
    syncQuitters(snapshot->quitters());
    syncTeamNames(snapshot->teamNames());
    syncViewerCount(snapshot->viewerCount());
}

// Only Client uses it via SnapshotSyncer
void SnapshotSyncer::initHero(const UDPMessageData& msgData) noexcept {
    KnightUPtr knight = KnightFactory::create(static_cast<GameObject::Type>(msgData.gameObjectType));
    //LOGI("DBG SnapshotSyncer::initHero msgData.id: %u", msgData.id);
    knight->setId(msgData.id);
    //LOGI("DBG SnapshotSyncer::initHero knight->id() %u", knight->id());
    updateHero(knight, msgData);
    world_->addKnight(std::move(knight));
    //debug("DBG knight has been created");
}

void SnapshotSyncer::updateGameObject(GameObject* gameObject, const UDPMessageData& msgData) noexcept {
    gameObject->setX(ZRD(msgData.x));
    gameObject->setFrameIndex(msgData.frameIndex);
    if(msgData.skinMin != 0xffff) {
        gameObject->setSkinMin(msgData.skinMin);
        gameObject->setSkinMax(msgData.skinMax);
    }
    //gameObject->setFlipped(msgData.flipped);
    //gameObject->setHitPoints(msgData.hitPoints, msgData.maxHitPoints);

    const MediaData* frameData = gameObject->frameData();
    const Rect& contentRect = frameData->contentRect();
    double y = ZRD(msgData.contentBottom) - contentRect.y - contentRect.height;
    gameObject->setY(y);
}

// full server snapshot calls it
void SnapshotSyncer::updateHero(KnightUPtr& knight, const UDPMessageData& msgData) noexcept {
    //debug("DBG SnapshotSyncer::updateHero 1");
    updateGameObject(knight.get(), msgData);
    knight->forceState(static_cast<State::Type>(msgData.state));
    if(static_cast<State::Type>(msgData.state) == State::Type::ST_IDLE && Application::environment()) {
        Application::environment()->setActiveKnightId(knight->id());
        if(Application::tileset()) {
            Application::tileset()->setActiveTeam(knight->type());
        }
    }
    //debug("DBG SnapshotSyncer::updateHero 2");
}

void SnapshotSyncer::initParticle(const UDPMessageData& msgData) noexcept {
    ParticleUPtr particle = std::make_unique<Particle>(0, 0, static_cast<GameObject::Type>(msgData.gameObjectType));
    particle->setId(msgData.id);
    updateParticle(particle, msgData);
    world_->addParticle(std::move(particle));
}

void SnapshotSyncer::updateParticle(ParticleUPtr& particle, const UDPMessageData& msgData) noexcept {
    updateGameObject(particle.get(), msgData);
}

void SnapshotSyncer::initRollingStone(const UDPMessageData& msgData) noexcept {
    RollingStoneUPtr rollingStone = std::make_unique<RollingStone>(0, 0, static_cast<GameObject::Type>(msgData.gameObjectType),
                                                                   static_cast<RollingStone::Color>(msgData.color));
    rollingStone->setId(msgData.id);
    updateRollingStone(rollingStone, msgData);
    world_->addRollingStone(std::move(rollingStone));
}

void SnapshotSyncer::updateRollingStone(RollingStoneUPtr& rollingStone, const UDPMessageData& msgData) noexcept {
    updateGameObject(rollingStone.get(), msgData);
    rollingStone->setColor(static_cast<RollingStone::Color>(msgData.color));
}

void SnapshotSyncer::initOtherElement(const UDPMessageData& msgData) noexcept {
    if(BlueTriangleUPtr otherElement = BlueTriangleFactory::create(static_cast<GameObject::Type>(msgData.gameObjectType))) {
        //BlueTriangleUPtr otherElement = std::unique_ptr<BlueTriangle>(blueTriangle);
        otherElement->setId(msgData.id);
        updateOtherElement(otherElement, msgData);
        world_->addOtherElement(std::move(otherElement));
    }
}

void SnapshotSyncer::updateOtherElement(BlueTriangleUPtr& otherElement, const UDPMessageData& msgData) noexcept {
    updateGameObject(otherElement.get(), msgData);
    if(otherElement->messagebox1()) {
        otherElement->messagebox1()->setMessage(static_cast<TranslateInterface::Key>(msgData.messageKey));
        otherElement->messagebox1()->setShowCountDown(msgData.showCountDown);
        otherElement->messagebox1()->setSpareTime(msgData.spareTime);
        otherElement->messagebox1()->setMessageString(msgData.messageString);
    } else if(otherElement->card()) {
        otherElement->card()->setTeamIndex(msgData.teamIndex);
        otherElement->card()->setAmount(msgData.amount);
        otherElement->card()->setVisible(msgData.visible);
        otherElement->card()->setCoolDown(msgData.coolDown);
    } else if(otherElement->questionmark()) {
        otherElement->questionmark()->setVisible(msgData.visible);
        otherElement->questionmark()->setLinkedGameObjectId(msgData.linkedGameObjectId);
    } else if(otherElement->matchResults()) {
        otherElement->matchResults()->setTeamNum(msgData.teamCount);
        otherElement->matchResults()->setElapsedTimes(msgData.elapsedTimes);
        otherElement->matchResults()->setScores(msgData.scores);
        otherElement->matchResults()->setLooserTeam(msgData.looserTeam);
    } else if(otherElement->emoteFigure()) {
        otherElement->emoteFigure()->setVisible(msgData.visible);
        otherElement->emoteFigure()->setAvatar(msgData.teamIndex);
    }
}

void SnapshotSyncer::initRPGBgElement(const UDPMessageData& msgData) noexcept {
    RPGBgElementUPtr rpgBgElement = std::make_unique<RPGBgElement>(0, 0, NATIVE_BITMAP_LAKE_01);
    rpgBgElement->setId(msgData.id);
    updateRPGBgElement(rpgBgElement, msgData);
    world_->addRPGBgElement(std::move(rpgBgElement));
}

void SnapshotSyncer::updateRPGBgElement(RPGBgElementUPtr& rpgBgElement, const UDPMessageData& msgData) noexcept {
    updateGameObject(rpgBgElement.get(), msgData);
}
