#ifndef TILESET_H
#define TILESET_H

#include <memory>
#include "Position.h"
#include <string>
#include "Size.h"
#include <set>
#include "Maze.h"
#include "MoveHistory.h"
#include "UDPServerSnapshot.h"
#include "RollingStoneInterface.h"
#include "GameObjectInterface.h"
#include "TilesetItemInfo.h"

class World;
using WorldUPtr = std::unique_ptr<World>;
enum BitmapId : int;
class Knight;
using KnightUPtr = std::unique_ptr<Knight>;

class GameObject;
class PlayWithSomeone;
class PlayWithFriend1;
class PlayWithFriend3;
class PlayWithFriend4;
class PlayWithFriend5;
class PlayWithFriend6;
class TilesetCardHandler;
class Multiplayer1;
class Multiplayer3;
class Multiplayer5;

// https://theasciicode.com.ar/extended-ascii-code/box-drawing-character-single-line-lower-left-corner-ascii-code-192.html
class Tileset {
public:
    static constexpr wchar_t emptyField = L' ';   // space: no bitmap used for that tile
    static constexpr wchar_t wall = L'*';         // *    : brick/rock
    static constexpr wchar_t nextLine = L'\n';    // \n   : next line
    static constexpr wchar_t hole = L'x';         // x    : pit/hole
    static constexpr wchar_t holeRed = L'X';      // X    : pit/hole
    static constexpr wchar_t holeBlue = L'y';     // y    : pit/hole
    static constexpr wchar_t ball = L'o';         // o    : ball (rolling stone), any knight can push it, but only the dark knight can push it to the dark hole
    static constexpr wchar_t ballRed = L'O';      // O    : ball (rolling stone), any knight can push it, but only the red knight can push it to the red hole
    static constexpr wchar_t ballBlue = L'ó';     // ó    : ball (rolling stone), any knight can push it to any hole

    static constexpr wchar_t cactus1 = L'c';      // c    : cactus1
    static constexpr wchar_t cactus2 = L'C';      // C    : cactus2
    static constexpr wchar_t palmTree1 = L't';    // t    : palm tree 1
    static constexpr wchar_t palmTree2 = L'T';    // T    : palm tree 2
    static constexpr wchar_t lake = L'l';         // l    : lake
    static constexpr wchar_t boat = L'b';         // b    : boat
    static constexpr wchar_t staticStone = L's';  // s    : static stone
    static constexpr wchar_t building = L'B';     // B    : building
    static constexpr wchar_t skull = L'S';        // S    : skull
    static constexpr wchar_t bush1 = L'a';        // a    : bush1
    static constexpr wchar_t bush2 = L'A';        // A    : bush2
    static constexpr wchar_t bush3 = L'd';        // d    : bush3
    static constexpr wchar_t redKnight = L'h';    // h    : red knight
    static constexpr wchar_t darkKnight = L'H';   // H    : dark knight
    static constexpr wchar_t blondeFemaleKnight = L'K';   // K    : blonde female knight
    static constexpr wchar_t greenFemaleKnight = L'G';   // K    : green female knight

    static constexpr wchar_t invalidField = L'-'; // -    : invalid field
    static constexpr wchar_t starTile = L'q';     // q    : tile with a star
    static constexpr wchar_t emptyTile = L'e';    // e    : light brown empty tile

    static constexpr wchar_t road_0  = L'─';        // ─    : road horizontal middle
    static constexpr wchar_t road_1  = L'R';        // R    : road right end
    static constexpr wchar_t road_2  = L'└';        // └    : road bottom left corner
    static constexpr wchar_t road_3  = L'|';        // |    : road vertical middle
    static constexpr wchar_t road_4  = L'┌';        // ┌    : road top right corner
    static constexpr wchar_t road_5  = L'█';        // █    : road middle tile
    static constexpr wchar_t road_6  = L'╔';        // ╔    : road top-left corner v2
    static constexpr wchar_t road_7  = L'╗';        // ╗    : road top-right corner v2
    static constexpr wchar_t road_8  = L'═';        // ═    : road top-middle v2
    static constexpr wchar_t road_9  = L'«';        // «    : road left-middle v2
    static constexpr wchar_t road_10 = L'»';        // »    : road right-middle v2
    static constexpr wchar_t road_11 = L'#';        // #    : road bottom-middle v2
    static constexpr wchar_t road_12 = L'╚';        // ╚    : road bottom-left corner v2
    static constexpr wchar_t road_13 = L'╝';        // ╝    : road bottom-right corner v2
    static constexpr wchar_t road_14 = L'1';        // 1    : road inverted bottom-left_corner v2,
    static constexpr wchar_t road_15 = L'2';        // 2    : road inverted bottom-right_corner v2,
    static constexpr wchar_t road_16 = L'3';        // 3    : road inverted top-left_corner v2,
    static constexpr wchar_t road_17 = L'4';        // 4    : road inverted top-right_corner v2,
    static constexpr wchar_t road_18 = L'5';        // 5    : road inverted center-corner v2,
    static constexpr wchar_t road_19 = L'6';        // 6    : road center-corner v2,

    // winner team variables
    static constexpr uint8_t WINNER_UNKNOWN = 255;
    static constexpr uint8_t MATCH_CANCELLED = 254;
    static constexpr uint8_t LOOSER_UNKNOWN = 255;

    Tileset() noexcept;
    virtual ~Tileset() noexcept = default;
    Tileset(const Tileset&) = delete;
    virtual int tileWidth() const noexcept {return tileSize_.width;}
    virtual int tileHeight() const noexcept {return tileSize_.height;}
    virtual void updateStonePosition(uint32_t id, int x, int y, RollingStoneInterface::Color color);
    virtual int testKnightXPosition(double x) const noexcept;
    virtual int testKnightYPosition(double y) const noexcept;
    virtual int getTileX(int x) const noexcept;
    virtual int getTileY(int y) const noexcept;
    virtual int convertKnightX2TileX(double x) const noexcept;
    virtual int convertKnightY2TileY(double y) const noexcept;
    virtual double convertTileX2KnightX(int x) const noexcept;
    virtual double convertTileY2KnightY(int y) const noexcept;
    virtual int convertStoneX2TileX(double x) const noexcept;
    virtual int convertStoneY2TileY(double y) const noexcept;
    virtual int convertTileX2StoneX(double x) const noexcept;
    virtual int convertTileY2StoneY(double y) const noexcept;
    virtual int convertWindowX2TileX(double x) const noexcept;
    virtual int convertWindowY2TileY(double y) const noexcept;
    virtual bool isBlockingTile(int x, int y, VectorD direction) const noexcept;
    virtual PositionI position() const noexcept {return position_;}
    virtual int width() const noexcept {return width_;}
    virtual int height()const noexcept {return height_;}
    virtual int widthInPixels() const noexcept {return (width_ - 1) * tileSize_.width;}
    virtual int heightInPixels()const noexcept {return height_ * tileSize_.height;}
    virtual void init(WorldUPtr& world) noexcept = 0;
    virtual void removeItem(uint32_t id);
    virtual wchar_t tileContent(int x, int y) const noexcept;
    virtual void setupStonHoleCollision(WorldUPtr& world) noexcept;
    virtual const Maze::MazeVector& getMazeVector(const Knight* knight, int heroX, int heroY);
    virtual int teamCount() const noexcept {return 0;}

    virtual void update(double now) noexcept;
    virtual void updateTexts([[maybe_unused]]double now) noexcept {}
    virtual void updateElapsedTime([[maybe_unused]]double now) noexcept {};
    virtual void teamCountCheck() noexcept {}
    virtual void updateTeamMarker([[maybe_unused]]double now) noexcept {}
    virtual void drawTexts() noexcept {}
    virtual void drawTexts(int levelIndex, int minimumPushCount) noexcept;

    virtual void followTheKnight() noexcept;

    // History is used by levels and multiplayer games too
    // Levels for undo a move, multiplayer for check the rules
    virtual void updateHistory(Knight*, MoveHistory::EventType, VectorD direction) noexcept;
    virtual void clearHistory() noexcept {moveHistory_.clear();}
    [[nodiscard]] virtual size_t historySize() const noexcept {return moveHistory_.size();}
    virtual bool walkHistoryRuleCheck() const noexcept {return true;}
    virtual void undoLastStep() noexcept;

    virtual void nextTurn() noexcept {}
    virtual bool isKnightInActiveTeam(Knight*) const noexcept {return true;}
    virtual void setActiveTeam(GameObjectInterface::Type) noexcept {}
    virtual void setActiveTeamId(unsigned char /*teamId*/) noexcept {}
    virtual bool isThereActiveTeam() const noexcept {return false;}
    virtual unsigned char activeTeamId() const noexcept {return 255;}
    virtual uint32_t gameObjectId(int tileX, int tileY) const noexcept;
    virtual void announceWinner([[maybe_unused]]int looserIndex) noexcept {}
    virtual void incStonePushCounter() noexcept;
    virtual int stonePushCounter() noexcept final {return stonePushCounter_;}
    virtual void resetPassCount() noexcept {}
    virtual void setLastTurn() noexcept {}
    virtual bool isTileOccupiedByAKnight(int x, int y) const noexcept;

#ifdef __SERVERAPP__
    [[nodiscard]] virtual bool playersJoined() const noexcept {return false;}
#endif
    // WorldTouchEvent forwards the field move coordinates
    virtual void move([[maybe_unused]]int deltaX, [[maybe_unused]]int deltaY) noexcept {}
    virtual int deltaX() const noexcept {return 0;}
    virtual int deltaY() const noexcept {return 0;}

    // MultiplayerWorld calls it
    virtual void recalcEveryXY(MultiplayerWorld*) noexcept {}
    virtual bool onGameObjectChange(GameObject*, UDPMessageData::Fields) noexcept {return false;}
    virtual uint32_t score() const noexcept {return 0;}
    virtual void setScore(uint32_t /*score*/) noexcept {}
    virtual uint16_t elapsedGameTime(uint8_t) noexcept {return 0;}
    virtual void setElapsedGameTime([[maybe_unused]]uint8_t team, [[maybe_unused]]uint16_t value) noexcept {}
    virtual bool gameOver() const noexcept {return false;}
    virtual uint8_t winnerTeam() const noexcept {return WINNER_UNKNOWN;}
    virtual void setWinnerTeam(uint8_t) noexcept{}
    virtual uint8_t viewOnly() const noexcept{return false;}
    virtual void setViewOnly(uint8_t) noexcept{}
    virtual void setQuitters(const std::vector<uint8_t>&) noexcept {}
    virtual void setTeamName([[maybe_unused]]uint8_t team, [[maybe_unused]]std::wstring name) noexcept {}

    // dynamic_cast
    virtual PlayWithSomeone* playWithSomeone() noexcept {return nullptr;}
    virtual PlayWithFriend1* playWithFriend1() noexcept {return nullptr;}
    virtual PlayWithFriend3* playWithFriend3() noexcept {return nullptr;}
    virtual PlayWithFriend4* playWithFriend4() noexcept {return nullptr;}
    virtual PlayWithFriend5* playWithFriend5() noexcept {return nullptr;}
    virtual PlayWithFriend6* playWithFriend6() noexcept {return nullptr;}
    virtual TilesetCardHandler* tilesetCardHandler() noexcept {return nullptr;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept {return nullptr;}
    virtual Multiplayer1* multiplayer1() noexcept {return nullptr;}
    virtual Multiplayer3* multiplayer3() noexcept {return nullptr;}
    virtual Multiplayer5* multiplayer5() noexcept {return nullptr;}

protected:
    virtual void initTile(wchar_t& tile, int& row, int& col, WorldUPtr& world) noexcept;
    virtual void calcTileMapRect();
    virtual void createRollingStone(WorldUPtr& world, RollingStoneInterface::Color color, int row, int col);
    virtual void createHole(WorldUPtr& world, BitmapId bitmapId, int row, int col);
    virtual void createKnight(WorldUPtr& world, GameObjectInterface::Type type, int row, int col);
    void tileContentPrint() const noexcept;
    void undoLastStep(const MoveHistory::Event& lastEvent, const KnightUPtr& knight) noexcept;
    void restoreMembersFromHistoryEvent(const MoveHistory::Event& lastEvent) noexcept;
    void syncHoleIdsAndHoleObjects() noexcept;
    void syncStoneIdsAndStoneObjects() noexcept;
    void restoreCardsFromHistoryEvent(const MoveHistory::Event& lastEvent) noexcept;
    void restoreDarkKnightsPositionFromHistoryEvent(const MoveHistory::Event& lastEvent) noexcept;

    std::wstring tileMap_;
    int width_; // colomn number
    int height_; // row number
    SizeI tileSize_;
    PositionI position_;
    VectorI knightV_;
    VectorI stoneV_;
    std::vector<TilesetItemInfo> items_;
    std::set<uint32_t> holeIds_;
    std::set<uint32_t> stoneIds_;
    Maze::MazeVector mazeVector_;
    int stonePushCounter_{};
    MoveHistory moveHistory_;
};

using TilesetUPtr = std::unique_ptr<Tileset>;

#endif // TILESET_H
