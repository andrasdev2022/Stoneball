#include "MoveHistory.h"

MoveHistory::MoveHistory()
{

}


void MoveHistory::replaceItemId(uint32_t id1, uint32_t id2, ReplaceType r) noexcept {
    for(auto& e : events_) {
        for(auto& i : e.items) {
            if(i.gameObjectId == id1) i.gameObjectId = id2;
        }

        if(r == ReplaceType::HOLE) {
            e.holeIds.erase(id1);
            e.holeIds.insert(id2);
        } else {
            e.stoneIds.erase(id1);
            e.stoneIds.insert(id2);
        }
    }
}
