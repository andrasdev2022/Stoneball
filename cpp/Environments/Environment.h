#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "GameMap.h"
#include "ObserverVictoryEvent.h"
#include "Tileset.h"
#include "StaticObjects.h"
#include <unordered_map>
#include <limits>

class World;
class Tileset;
class RollingStone;
class GameObject;
class Knight;
using KnightUPtr = std::unique_ptr<Knight>;

class Environment : public ObserverVictoryEvent {
public:
    static constexpr uint32_t NONE_ACTIVE_KNIGHT = std::numeric_limits<uint32_t>::max();
    virtual void init(std::unique_ptr<World>& world, TilesetUPtr&& tileset ) noexcept;
    // Environment
    virtual GameMap* gameMap() noexcept {return gameMap_.get();}
    virtual StaticObjects* gravity() noexcept {return gravity_.get();}
    virtual Tileset* tileset() const noexcept {return tileset_.get();}
    virtual const std::vector<int>& islandIds() const noexcept {return islandIds_;}

    // ObserverVictoryEvent
    void notify(ObserverSubjectVictoryEvent* subject) override;

    virtual void setActiveKnightId(uint32_t id) noexcept;
    virtual uint32_t activeKnightId() const noexcept {return activeKnightId_;}

    virtual void incScore(int id) noexcept;
    virtual int score(int id) const noexcept;

    virtual void setLastStoodUpKnightId(uint32_t id) noexcept {lastStoodUpKnightId_ = id;}

protected:
    std::vector<int> islandIds_;
    TilesetUPtr tileset_;
    GameMapUPtr gameMap_;
    StaticObjectsUPtr gravity_;
    uint32_t activeKnightId_{NONE_ACTIVE_KNIGHT};
    std::unordered_map<int, int> score_; // TODO: should be based on teamid. Do I need this in single player mode?

private:
    bool removeHole(RollingStone* stone) noexcept;
    void finishGoalKick(const KnightUPtr& k) noexcept;

    double holeX_;
    double holeY_;
    uint32_t lastStoodUpKnightId_{0xffffffff};
    // These should be here?
    //MoveHistory moveHistory_;
    //Team activeTeam_{Team::LAST_ONE};
    //Team lastActiveTeam_{Team::LAST_ONE};

};

using EnvironmentUPtr = std::unique_ptr<Environment>;

#endif // __ENVIRONMENT_H__
