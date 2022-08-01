#ifndef __MOVEHISTORY_H__
#define __MOVEHISTORY_H__

#include <vector>
#include "Vector.h"
#include <string>
#include <set>
#include "TilesetItemInfo.h"
#include <map>

class MoveHistory
{
public:
    enum class EventType {
        PUSH,
        MOVE,
        UNDO_POINT
    };

    enum class ReplaceType {
        STONE,
        HOLE
    };

    struct Event {
        uint32_t gameObjectId; // knight id
        Position<double> position; // position of the knight
        EventType type;
        VectorD direction;

        // Tileset will fill these
        std::wstring tileMap;
        std::vector<TilesetItemInfo> items;
        std::set<uint32_t> holeIds;
        std::set<uint32_t> stoneIds;
        int stonePushCounter{};
        PositionI tilesetPosition;
        std::vector<uint8_t> cardsAmount;
        std::map<uint32_t, PositionD> darkKnights; // dark knights
    };
    MoveHistory();
    void addEvent(Event e) noexcept {events_.push_back(e);}
    void dropLastEvent() noexcept {if(events_.size()) events_.erase(events_.end() - 1);}
    void clear() noexcept {events_.clear();}
    [[nodiscard]]size_t size() const noexcept {return events_.size();}
    [[nodiscard]] const std::vector<Event>& events() const {return events_;}
    void replaceItemId(uint32_t id1, uint32_t id2, ReplaceType r) noexcept;

private:
    std::vector<Event> events_;
};

#endif // __MOVEHISTORY_H__
