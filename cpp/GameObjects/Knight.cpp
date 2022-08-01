#include "Knight.h"
#include "Visitor.h"
#include "StateKnightIdle.h"
#include "StateKnightRun.h"
#include "StateKnightSit.h"
#include "GameObject.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include "MediaData.h"
#include "StaticObjects.h"
#include "Application.h"
#include "Button.h"
#include "RollingStone.h"
#include "Vector.h"
#include "Tileset.h"
#include "Level1.h"
#include "Level2.h"
#include "Environment.h"
#include "UDPMessageData.h"
#include "Memory.h"
#include "Serialize.h"

namespace {
    class GameObjectVisitor final : public Visitor {
    public:
       GameObjectVisitor() noexcept : ptr(nullptr) {}
       void visit(StateKnightIdle* v)   noexcept override {reset(); ptr = v; idlePtr = v; frameData = v->frameData();}
       void visit(StateKnightRun* v)    noexcept override {reset(); ptr = v; runPtr = v; frameData = v->frameData();}
       void visit(StateKnightSit* v)    noexcept override {reset(); ptr = v; sitPtr = v; frameData = v->frameData();}
       void visit(GameObject* v)        noexcept override {reset(); ptr = v; frameData = v->frameData();}
       void reset() noexcept {
           ptr       = nullptr;
           frameData = nullptr;
           idlePtr   = nullptr;
           runPtr    = nullptr;
           sitPtr    = nullptr;
       }
       GameObject* ptr;
       StateKnightIdle* idlePtr;
       StateKnightRun* runPtr;
       StateKnightSit* sitPtr;
       const MediaData* frameData;
    };
    GameObjectVisitor gameObjectVisitor;
}

Knight::Knight(double x, double bottomY, GameObject::Type heroType) noexcept
: GameObject(x, bottomY, heroType)
, action(-1)
, buttonEventSender(nullptr)
, stateChangeButton(ButtonEventListener::ButtonState::BS_UP)
, died_(false)
, lastAttackTime_(-1)
, gravity_(nullptr)
, maxHeightOfJump_(0.)
, anyButtonEventFiredTimeStamp_(lastUpdateTmps)
{
    init(); // to fill the basic stuff like idle state
    //if(!state()) setState(new StateKnightIdle(x, contentBottom(), type_, 1.));
    if(!state()) setState(new StateKnightSit(x, contentBottom(), type_));
}

Knight::Knight(const Knight& z) noexcept
: GameObject(z)
, action(z.action)
, buttonEventSender(z.buttonEventSender)
, stateChangeButton(z.stateChangeButton)
, died_(z.died_)
, lastAttackTime_(z.lastAttackTime_)
, gravity_(z.gravity_)
, maxHeightOfJump_(z.maxHeightOfJump_)
, anyButtonEventFiredTimeStamp_(z.anyButtonEventFiredTimeStamp_)
, stopAt_(z.stopAt_)
{
    if(z.state()) setState(z.state()->copy());
    if(z.strategy_) setStrategy(z.strategy_->copy());
}

void Knight::update(double now) noexcept {
    if(strategy_) {
        strategy_->doActions(this);
    }
    state()->accept(&gameObjectVisitor);

    double oldX = gameObjectVisitor.ptr->x();
    double oldY = gameObjectVisitor.ptr->y();

    gameObjectVisitor.ptr->update(now);

    if(isSliding()) {
        //debug("Knight::update isStopppppppp");
        bool stop = false;
        if(direction_.x > .5 && (x() > oldX && x() > stopAt_.x)) {
            stop = true;
        } else if(direction_.x < -.5 && (x() < oldX && x() < stopAt_.x)) {
            stop = true;
        } else if(direction_.y > .5 && (y() > oldY && y() > stopAt_.y)) {
            stop = true;
        } else if(direction_.y < -.5 && (y() < oldY && y() < stopAt_.y)) {
            stop = true;
        }
        if(stop) {
            debug("Knight::update STOP");
            direction_ = VectorD();
            gameObjectVisitor.ptr->setX(stopAt_.x);
            gameObjectVisitor.ptr->setY(stopAt_.y);
            gameObjectVisitor.ptr->setDirectionX(0.);
            gameObjectVisitor.ptr->setDirectionY(0.);
            gameObjectVisitor.ptr->setVelocity(0.);
            velocity_ = 0.;
            stopAt_ = VectorD();
        }
    }

    if(gravity_) {
        if(lastUpdateTmps < 1e-4) {
            lastUpdateTmps = now;
        }
        // Collision test with Islands
        if(gravity_->detectRectCollision(this, 0, 0)) {
            // new position will be the old one, from before the update
            gameObjectVisitor.ptr->setX(oldX);
            gameObjectVisitor.ptr->setY(oldY);
        }
    }

    if(gameObjectVisitor.runPtr) {
        // History should be recorded
        VectorD d{gameObjectVisitor.runPtr->directionX(), gameObjectVisitor.runPtr->directionY()};
        Application::tileset()->updateHistory(this, MoveHistory::EventType::MOVE, d);
    }

    lastUpdateTmps = now;
}

void Knight::changeState() noexcept {
    auto prevStateType = state()->stateType();
    if(state()->isReady()) {
        state()->accept(&gameObjectVisitor);
        if(gameObjectVisitor.runPtr) {
            gameObjectVisitor.runPtr->keepRunning(!!strategy_);
        }
        state()->change(this);
    } else if(died_) {
        state()->change(this);
        buttonEventSender = nullptr;
        died_ = false;
    } else if(buttonEventSender) {
        state()->change(this);
        state()->accept(&gameObjectVisitor);
        gameObjectVisitor.ptr->onButtonEvent(buttonEventSender, stateChangeButton);
        buttonEventSender = nullptr;
    }
    auto newStateType = state()->stateType();
    if(prevStateType != newStateType) {
        if(newStateType == State::Type::ST_IDLE) {
        }
    }
}

void Knight::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    //LOGI("Knight::draw 1");
    state()->accept(&gameObjectVisitor);
    //LOGI("Knight::draw 2: %x", v.ptr);
    // Refresh the hitpoints for the actual state
    gameObjectVisitor.ptr->setHitPoints(hitPoints_, maxHitPoints_);
    gameObjectVisitor.ptr->draw(info, pixels);
    //LOGI("Knight::draw 3");
}

const MediaData* Knight::frameData() const noexcept {
    gameObjectVisitor.frameData = nullptr;
    if(state()) state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.frameData;
}

const GameObjectDescriptor& Knight::god() const noexcept {
    /*if(state()) {
        gameObjectVisitor.reset();
        state()->accept(&gameObjectVisitor);
        if(gameObjectVisitor.ptr) gameObjectVisitor.ptr->god();
    }*/
    return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
}

// ButtonEventListener
bool Knight::onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept {
    //debug("DBG Knight::onButtonEvent buttonState: %d", buttonState);
    buttonEventSender = sender; //(GameObject*)state().get();
    state()->accept(&gameObjectVisitor);
    if(gameObjectVisitor.ptr->onButtonEvent(sender, buttonState)) {
        stateChangeButton = buttonState;
        anyButtonEventFiredTimeStamp_ = lastUpdateTmps;
        // the knights and the stones must not be in walking status
        if(Application::isUndoCompatible() && stateChangeButton == ButtonState::BS_DOWN) {
            //debug("Knight::onButtonEvent UNDO_POINT");
            Application::tileset()->updateHistory(this, MoveHistory::EventType::UNDO_POINT, VectorD());
        }
        return true;
        //LOGI("DBG Knight::onButtonEvent %f", anyButtonEventFiredTimeStamp_);
    } else {
        buttonEventSender = nullptr;
    }
    return false;
}

double Knight::x() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->x();
}

double Knight::y() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->y();
}

int Knight::width() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->width();
}

int Knight::height() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->height();
}

bool Knight::isIdle() const noexcept {
    gameObjectVisitor.reset();
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.idlePtr;
}

bool Knight::isRunning() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.runPtr;
}

void Knight::die() noexcept {
    died_ = true;
    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->die();
}

bool Knight::isDead() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->isDead();
}

void Knight::setX(double x) noexcept {
    ::GameObject::setX(x);
    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->setX(x);
}

void Knight::setY(double y) noexcept {
    ::GameObject::setY(y);
    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->setY(y);
}

void Knight::yCorrectionByGravity(double newY, bool collision) noexcept {
    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->yCorrectionByGravity(newY, collision);
}

int Knight::frameIndex() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->frameIndex();
}

void Knight::setHitPoints(int hitPoints, int maxHitPoints) noexcept {
    GameObject::setHitPoints(hitPoints, maxHitPoints);
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->setHitPoints(hitPoints, maxHitPoints);
}

void Knight::setFrameIndex(int frameIndex) noexcept {
    GameObject::setFrameIndex(frameIndex);
    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->setFrameIndex(frameIndex);
}

double Knight::velocity() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->velocity();
}

void Knight::wakeUp(double now) noexcept {
    GameObject::wakeUp(now);
    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->wakeUp(now);
}

bool Knight::flipped() const noexcept {
    if(!state()) return false;
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->flipped();
}

void Knight::setFlipped(bool f) noexcept {
    if(!state()) return;
    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->setFlipped(f);
}

int Knight::skinMax() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->skinMax();
}

int Knight::skinMin() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->skinMin();
}

void Knight::setSkinMax(int skinMax) noexcept {
    ::GameObject::setSkinMax(skinMax);
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->setSkinMax(skinMax);
}

void Knight::setSkinMin(int skinMin) noexcept {
    ::GameObject::setSkinMin(skinMin);
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->setSkinMin(skinMin);
}

// Only the MultiplayerWorld calls it
bool Knight::respawn() noexcept {
    gameObjectVisitor.reset();
    setX(0._ZRD);
    const Rect& contentRect = frameData()->contentRect();
    double y = 8700._ZRH - contentRect.y - contentRect.height;
    setY(y);
    setHitPoints(4000, 4000);
    setMaxHeightOfJump(200._ZRD);
    //state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->respawn();
    return true;
}

double Knight::directionX() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->directionX();
}

double Knight::directionY() const noexcept {
    state()->accept(&gameObjectVisitor);
    return gameObjectVisitor.ptr->directionY();
}

void Knight::handleCollision(RollingStone* rollingStone) noexcept {
    const Rect heroRect = collisionData()->rect();
    const Rect rollingStoneRect = rollingStone->collisionData()->rect();

    Vector v(x() + heroRect.x + heroRect.width / 2,
             y() + heroRect.y + heroRect.height / 2,
             rollingStone->x() + rollingStoneRect.x + rollingStoneRect.width / 2,
             rollingStone->y() + rollingStoneRect.y + rollingStoneRect.height / 2);

    double directionX = 0.;
    double directionY = 0.;
    if(Math::abs(v.x) > Math::abs(v.y)) {
        if(v.x < -1.e-3) directionX = -1.;
        else if(v.x > 1.e-3) directionX = 1.;
    } else {
        if(v.y < -1.e-3) directionY = -1.;
        else if(v.y > 1.e-3) directionY = 1.;
    }

//    std::cout << "directionX: " << directionX << ", " << "directionY: " << directionY << "\n";
//    std::cout << "v.x: " << v.x << ", " << "v.y: " << v.y << "\n";

    Tileset* tileset = Application::tileset();
    rollingStone->push(directionX * tileset->tileWidth(), directionY * tileset->tileHeight(), velocity());
    // if the knight is pushed by another kight (being in sitting state) then we do not touch the history
    if(isIdle() || isRunning()) {
        tileset->updateHistory(this, MoveHistory::EventType::PUSH, VectorD{directionX, directionY});
    }
}

// For Multiplayer in order to have it
void Knight::forceState(State::Type type) noexcept {
    if(state()->stateType() == type) return;

    gameObjectVisitor.reset();
    state()->accept(&gameObjectVisitor);

    bool flippedV = flipped();
    switch(type) {
    case State::Type::ST_IDLE:
        setState(new StateKnightIdle(x(), gameObjectVisitor.ptr->contentBottom(), type_, gameObjectVisitor.ptr->directionX()));
    break;
    case State::Type::ST_WALK:break;
    case State::Type::ST_RUN:
        if(flippedV)
            setState(new StateKnightRun(x(), gameObjectVisitor.ptr->contentBottom(), type_, Direction::EAST));
        else
            setState(new StateKnightRun(x(), gameObjectVisitor.ptr->contentBottom(), type_, Direction::WEST));
    break;
    case State::Type::ST_JUMP: break;
    case State::Type::ST_FALL: break;
    case State::Type::ST_MOVE: break;
    case State::Type::ST_ROLL: break;
    case State::Type::ST_SIT:
        setState(new StateKnightSit(x(), gameObjectVisitor.ptr->contentBottom(), type_));
        break;
    case State::Type::ST_ATTACK1:
    case State::Type::ST_ATTACK2:
    case State::Type::ST_ATTACK3:
    case State::Type::ST_DEAD:
    case State::Type::ST_HURT:
    case State::Type::ST_BLOCK1:
    break;
    case State::Type::ST_FLY:break;
    case State::Type::COUNT:break;
    }
    setFlipped(flippedV);
}

bool Knight::isReady() const {
    return state()->isReady();
}

void Knight::sitDown() {
    ::GameObject::setX(x());
    ::GameObject::setY(y());
    ::GameObject::setFrameIndex(frameIndex());
    // we must not remove the strategy here
    //setStrategy(nullptr);
    forceState(State::Type::ST_SIT);
}

void Knight::getUp() {
    ::GameObject::setX(x());
    ::GameObject::setY(y());
    ::GameObject::setFrameIndex(frameIndex());
    setStrategy(nullptr);
    forceState(State::Type::ST_IDLE);
    if(Application::environment()) {
        Application::environment()->setLastStoodUpKnightId(id());
    }
}

bool Knight::push(double directionX, double directionY, double velocity) {
    if(isSliding()) return false;

    //debug("Oh no!! I'm pushed!");

    state()->accept(&gameObjectVisitor);
    gameObjectVisitor.ptr->setDirectionX(directionX);
    gameObjectVisitor.ptr->setDirectionY(directionY);
    gameObjectVisitor.ptr->setVelocity(velocity);

    direction_.x = directionX;
    direction_.y = directionY;

    stopAt_.x = x() + (directionX > 1e-3 ? Application::tileset()->tileWidth() : (directionX < -1e-3 ? - Application::tileset()->tileWidth() : 0.));
    stopAt_.y = y() + (directionY > 1e-3 ? Application::tileset()->tileHeight() : (directionY < -1e-3 ? - Application::tileset()->tileHeight() : 0.));

    //debug("directionX: %f, directionY: %f", directionX, directionY);
    //debug("x: %f, y: %f, stopAt.x: %f, stopAt.y: %f", x(), y(), stopAt_.x, stopAt_.y);

    Application::tileset()->updateHistory(this, MoveHistory::EventType::PUSH, VectorD{directionX, directionY});

    return true;
}

bool Knight::addData(Serialize* serialize) {
    return GameObject::addData(serialize) &&
           serialize->add((uint8_t)'K') != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)state()->stateType()) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* Knight::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    //src = GameObject::buildFrom(messageData, src);
    //src = Memory::copy(messageData->type, src);
    src = Memory::copy(messageData->state, src);
    return src;
}
