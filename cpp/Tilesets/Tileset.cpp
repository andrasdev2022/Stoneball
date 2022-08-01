#include "Tileset.h"
#include "Application.h"
#include <algorithm>
#include "MediaData.h"
#include "StaticObjects.h"
#include "Global.h"
#include "Algorithms.h"
#include "Environment.h"
#include "KnightFactory.h"
#include "Screen.h"
#include "RPGBgElement.h"
#include "World.h"
#include "BitmapIds.h"
#include "Card.h"
#include "StringUtil.h"

Tileset::Tileset() noexcept
: width_(0)
, height_(0)
, tileSize_(32_ZRD, 24_ZRD)
, position_(0,0)
, knightV_(0, -10_ZRD)
, stoneV_(0, -9_ZRD)
{
}

// space: no bitmap used for that tile
// *    : brick/rock
// \n   : next line
// x    : pit/hole
// o    : ball (rolling stone)
// c    : cactus1
// C    : cactus2
// t    : palm tree 1
// T    : palm tree 2
// l    : lake
// b    : boat
// s    : static stone
// B    : building
// S    : skull
// a    : bush1
// A    : bush2
// d    : bush3
// ─    : road horizontal middle
// R    : road right end
// └    : road bottom left corner
// |    : road vertical middle
// ┌    : road top right corner
// h    : knight
// █    : road middle tile
// ╔    : road top-left corner v2
// ╗    : road top-right corner v2
// ═    : road top-middle v2
// «    : road left-middle v2
// »    : road right-middle v2
// #    : road bottom-middle v2
// ╚    : road bottom-left corner v2
// ╝    : road bottom-right corner v2
// 1    : road inverted bottom-left_corner v2,
// 2    : road inverted bottom-right_corner v2,
// 3    : road inverted top-left_corner v2,
// 4    : road inverted top-right_corner v2,
// 5    : road inverted center-corner v2,
// 6    : road center-corner v2,
void Tileset::initTile(wchar_t& tile, int& row, int& col, WorldUPtr& world) noexcept {
    Environment* environment = Application::environment();
    GameMap* gameMap = environment->gameMap();
    auto gravity = environment->gravity();

    const int roadYOffset = position_.y + (row + 1) * tileSize_.height + 10_ZRD;
    switch(tile) {
    case Tileset::emptyField:
        ++col;
        break;
    case Tileset::wall: {
            int id = gameMap->createIsland(Island::Type::Wall04, position_.x + col * tileSize_.width, position_.y + row * tileSize_.height);
            gravity->addIslandRects(gameMap->island(id)->rects());
            ++col;
        }
        break;
    case Tileset::skull: {
            gameMap->createIsland(Island::Type::Skull, position_.x + col * tileSize_.width, position_.y + row * tileSize_.height);
            ++col;
        }
        break;
    case Tileset::bush1: {
            gameMap->createIsland(Island::Type::Bush1, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_BUSH_01));
            ++col;
        }
        break;
    case Tileset::bush2: {
            gameMap->createIsland(Island::Type::Bush2, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_BUSH_02));
            ++col;
        }
        break;
    case Tileset::bush3: {
            gameMap->createIsland(Island::Type::Bush3, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_BUSH_03));
            ++col;
        }
        break;
    case Tileset::starTile: {
            gameMap->createIsland(Island::Type::STARTILE, position_.x + col * tileSize_.width, position_.y + row * tileSize_.height + 11_ZRD);
            ++col;
        }
        break;
    case Tileset::emptyTile: {
            gameMap->createIsland(Island::Type::EMPTYTILE, position_.x + col * tileSize_.width, position_.y + row * tileSize_.height + 11_ZRD);
            ++col;
        }
        break;
    case Tileset::road_0: { // road
            gameMap->createIsland(Island::Type::ROAD_HORIZONTAL_MIDDLE, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_ROAD_HORIZONTAL_MIDDLE));
            ++col;
        }
        break;
    case Tileset::road_1: { // road
            gameMap->createIsland(Island::Type::ROAD_RIGHT_END, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_ROAD_RIGHT_END));
            ++col;
        }
        break;
    case Tileset::road_2: { // road
            gameMap->createIsland(Island::Type::ROAD_BOTTOM_LEFT_CORNER, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_ROAD_BOTTOM_LEFT_CORNER));
            ++col;
        }
        break;
    case Tileset::road_3: { // road
            gameMap->createIsland(Island::Type::ROAD_VERTICAL_MIDDLE, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_ROAD_VERTICAL_MIDDLE));
            ++col;
        }
        break;
    case Tileset::road_4: { // road
            gameMap->createIsland(Island::Type::ROAD_TOP_LEFT_CORNER, position_.x + col * tileSize_.width, position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_ROAD_TOP_LEFT_CORNER));
            ++col;
        }
        break;
    case Tileset::road_5: { // road
            gameMap->createIsland(Island::Type::ROAD_MIDDLE, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_MIDDLE));
            ++col;
        }
        break;
    case Tileset::road_6: { // road
            gameMap->createIsland(Island::Type::ROAD_TOP_LEFT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_TOP_LEFT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_7: { // road
            gameMap->createIsland(Island::Type::ROAD_TOP_RIGHT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_TOP_RIGHT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_8: { // road
            gameMap->createIsland(Island::Type::ROAD_TOP_MIDDLE_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_TOP_MIDDLE_V2));
            ++col;
        }
        break;
    case Tileset::road_9: { // road
            gameMap->createIsland(Island::Type::ROAD_LEFT_MIDDLE_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_LEFT_MIDDLE_V2));
            ++col;
        }
        break;
    case Tileset::road_10: { // road
            gameMap->createIsland(Island::Type::ROAD_RIGHT_MIDDLE_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_RIGHT_MIDDLE_V2));
            ++col;
        }
        break;
    case Tileset::road_11: { // road
            gameMap->createIsland(Island::Type::ROAD_BOTTOM_MIDDLE_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_BOTTOM_MIDDLE_V2));
            ++col;
        }
        break;
    case Tileset::road_12: { // road bottom-left corner v2
            gameMap->createIsland(Island::Type::ROAD_BOTTOM_LEFT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_BOTTOM_LEFT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_13: { // road bottom-right corner v2
            gameMap->createIsland(Island::Type::ROAD_BOTTOM_RIGHT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_BOTTOM_RIGHT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_14: { // road inverted bottom-left_corner v2,
            gameMap->createIsland(Island::Type::ROAD_INVERTED_BOTTOM_LEFT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_INVERTED_BOTTOM_LEFT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_15: { // road inverted bottom-right_corner v2,
            gameMap->createIsland(Island::Type::ROAD_INVERTED_BOTTOM_RIGHT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_INVERTED_BOTTOM_RIGHT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_16: { // road inverted top-left_corner v2,
            gameMap->createIsland(Island::Type::ROAD_INVERTED_TOP_LEFT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_INVERTED_TOP_LEFT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_17: { // road inverted top-right_corner v2,
            gameMap->createIsland(Island::Type::ROAD_INVERTED_TOP_RIGHT_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_INVERTED_TOP_RIGHT_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_18: { // road inverted center-corner v2,
            gameMap->createIsland(Island::Type::ROAD_INVERTED_CENTER_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_INVERTED_CENTER_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::road_19: { // road center-corner v2,
            gameMap->createIsland(Island::Type::ROAD_CENTER_CORNER_V2, position_.x + col * tileSize_.width, roadYOffset - bitmapHeight(NATIVE_BITMAP_ROAD_CENTER_CORNER_V2));
            ++col;
        }
        break;
    case Tileset::nextLine:
        col = 0;
        ++row;
        break;
    case Tileset::hole: {
        createHole(world, NATIVE_BITMAP_HOLE_DARK_01, row, col);
            ++col;
        }
        break;
    case Tileset::holeRed: {
        createHole(world, NATIVE_BITMAP_HOLE_RED_01, row, col);
            ++col;
        }
        break;
    case Tileset::holeBlue: {
        createHole(world, NATIVE_BITMAP_HOLE_BLUE_01, row, col);
            ++col;
        }
        break;
    case Tileset::ball: {
            createRollingStone(world, RollingStoneInterface::Color::Gray, row, col);
            ++col;
        }
        break;
    case Tileset::ballRed: {
            createRollingStone(world, RollingStoneInterface::Color::Red, row, col);
            ++col;
        }
        break;
    case Tileset::ballBlue: {
            createRollingStone(world, RollingStoneInterface::Color::Blue, row, col);
            ++col;
        }
        break;
    case Tileset::cactus1: {
            RPGBgElementUPtr cactus = std::make_unique<RPGBgElement>(32_ZRW, 32_ZRH, NATIVE_BITMAP_CACTUS_01);
            cactus->setX(position_.x + col * tileSize_.width);
            cactus->setY(position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_CACTUS_01));
            items_.emplace_back(TilesetItemInfo{cactus->id(), PositionI{col, row}, static_cast<BitmapId>(cactus->skinMin())});
            world->addRPGBgElement(std::move(cactus));
            ++col;
        }
        break;
    case Tileset::cactus2: {
            RPGBgElementUPtr cactus = std::make_unique<RPGBgElement>(28_ZRW, 186_ZRH, NATIVE_BITMAP_CACTUS_02);
            cactus->setX(position_.x + col * tileSize_.width);
            cactus->setY(position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_CACTUS_02));
            items_.emplace_back(TilesetItemInfo{cactus->id(), PositionI{col, row}, static_cast<BitmapId>(cactus->skinMin())});
            world->addRPGBgElement(std::move(cactus));
            ++col;
        }
        break;
    case Tileset::palmTree1: {
            RPGBgElementUPtr tree = std::make_unique<RPGBgElement>(288_ZRW, 108_ZRH, NATIVE_BITMAP_TREE_01);
            tree->setX(position_.x + col * tileSize_.width - 38_ZRD);
            tree->setY(position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_TREE_01));
            items_.emplace_back(TilesetItemInfo{tree->id(), PositionI{col, row}, static_cast<BitmapId>(tree->skinMin())});
            world->addRPGBgElement(std::move(tree));
            ++col;
        }
        break;
    case Tileset::lake: {
            gameMap->createIsland(Island::Type::Lake, position_.x + col * tileSize_.width, position_.y + row * tileSize_.height);
            ++col;
        }
        break;
    case Tileset::palmTree2: {
            RPGBgElementUPtr tree = std::make_unique<RPGBgElement>(375_ZRW, 140_ZRH, NATIVE_BITMAP_TREE_02);
            tree->setX(position_.x + col * tileSize_.width - 30_ZRD);
            tree->setY(position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_TREE_02) - 5_ZRD);
            items_.emplace_back(TilesetItemInfo{tree->id(), PositionI{col, row}, static_cast<BitmapId>(tree->skinMin())});
            world->addRPGBgElement(std::move(tree));
            ++col;
        }
        break;
    case Tileset::boat: {
            RPGBgElementUPtr boat = std::make_unique<RPGBgElement>(345_ZRW, 180_ZRH, NATIVE_BITMAP_BOAT);
            boat->setX(position_.x + col * tileSize_.width);
            boat->setY(position_.y + row * tileSize_.height);
            items_.emplace_back(TilesetItemInfo{boat->id(), PositionI{col, row}, static_cast<BitmapId>(boat->skinMin())});
            world->addRPGBgElement(std::move(boat));
            ++col;
        }
        break;
    case Tileset::staticStone: {
            RPGBgElementUPtr stone = std::make_unique<RPGBgElement>(268_ZRW, 80_ZRH, NATIVE_BITMAP_STONE_01);
            stone->setX(position_.x + col * tileSize_.width);
            stone->setY(position_.y + row * tileSize_.height);
            items_.emplace_back(TilesetItemInfo{stone->id(), PositionI{col, row}, static_cast<BitmapId>(stone->skinMin())});
            world->addRPGBgElement(std::move(stone));
            ++col;
        }
        break;
    case Tileset::building: {
            RPGBgElementUPtr building = std::make_unique<RPGBgElement>(424_ZRW, 0_ZRH, NATIVE_BITMAP_BUILDING_01);
            building->setX(position_.x + col * tileSize_.width + 10_ZRD);
            building->setY(position_.y + (row + 1) * tileSize_.height - bitmapHeight(NATIVE_BITMAP_BUILDING_01));
            items_.emplace_back(TilesetItemInfo{building->id(), PositionI{col, row}, static_cast<BitmapId>(building->skinMin())});
            world->addRPGBgElement(std::move(building));
            ++col;
        }
        break;
    case Tileset::redKnight: {
            createKnight(world, GameObjectInterface::Type::GOT_KNIGHT_RED, row, col);
            tile = emptyField; // do not store the hero on the map
            ++col;
        }
        break;
    case Tileset::darkKnight: {
            createKnight(world, GameObjectInterface::Type::GOT_KNIGHT_DARK, row, col);
            tile = emptyField; // do not store the hero on the map
            ++col;
        }
        break;
    case Tileset::blondeFemaleKnight: {
            createKnight(world, GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE, row, col);
            tile = emptyField; // do not store the hero on the map
            ++col;
        }
        break;
    case Tileset::greenFemaleKnight: {
            createKnight(world, GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE, row, col);
            tile = emptyField; // do not store the hero on the map
            ++col;
        }
        break;
    case Tileset::invalidField:
        ++col;
        break;
    };
    //if(col > width_) width_ = col;
    //if(row > height_) height_ = row;
}

int Tileset::testKnightXPosition(double x) const noexcept {
    return ((int)round(x - position_.x - knightV_.x) % tileSize_.width);
}

int Tileset::testKnightYPosition(double y) const noexcept {
    return ((int)round(y - position_.y - knightV_.y) % tileSize_.height);
}

int Tileset::convertKnightX2TileX(double x) const noexcept {
    return floor((x - position_.x - knightV_.x) / tileSize_.width);
}

int Tileset::convertKnightY2TileY(double y) const noexcept {
    return floor((y - position_.y - knightV_.y) / tileSize_.height);
    //std::cout << y << ", " << position_.y << ", " << knightV_.y << "\n";
}

double Tileset::convertTileX2KnightX(int x) const noexcept {
    return position_.x + x * tileSize_.width + knightV_.x;
}

double Tileset::convertTileY2KnightY(int y) const noexcept {
    return position_.y + y * tileSize_.height + knightV_.y;
}

int Tileset::convertStoneX2TileX(double x) const noexcept {
    return floor((x - position_.x - stoneV_.x) / tileSize_.width);
}

int Tileset::convertStoneY2TileY(double y) const noexcept {
    return floor((y - position_.y - stoneV_.y) / tileSize_.height);
}

int Tileset::convertTileX2StoneX(double x) const noexcept {
    return position_.x + x * tileSize_.width + stoneV_.x;
}

int Tileset::convertTileY2StoneY(double y) const noexcept {
    return position_.y + y * tileSize_.height + stoneV_.y;
}

int Tileset::convertWindowX2TileX(double x) const noexcept {
    return floor((x - position_.x) / tileSize_.width);
}

int Tileset::convertWindowY2TileY(double y) const noexcept {
    return floor((y - position_.y) / tileSize_.height);
}

void Tileset::tileContentPrint() const noexcept {
    for(int i = 0; i < height_; ++i) {
        for(int j = 0; j < width_; ++j) {
            std::wcout << tileMap_.at(width_ * i + j);
        }
        std::wcout << L"\n";
    }
}

wchar_t Tileset::tileContent(int x, int y) const noexcept {
    // The new line colum is invalid too
    if(x < 0 || x >= width_ - 1) return Tileset::invalidField;
    if(y < 0 || y >= height_) return Tileset::invalidField;
    return tileMap_.at(width_ * y + x);
}

int Tileset::getTileX(int x) const noexcept {
    return position_.x + x * tileSize_.width + knightV_.x;
}

int Tileset::getTileY(int y) const noexcept {
    return position_.y + y * tileSize_.height + knightV_.y;
}

bool Tileset::isBlockingTile(int x, int y, VectorD direction) const noexcept {
    wchar_t tile = tileContent(x, y);
    if(Algorithms::anyOf(tile, Tileset::wall, Tileset::hole, Tileset::holeRed, Tileset::holeBlue, Tileset::cactus1,
                         Tileset::cactus2, Tileset::invalidField,
                         Tileset::palmTree1, Tileset::palmTree2, Tileset::staticStone, Tileset::building)) {
        return true;
    }

    //std::cout << "Level1::isBlockingTile x:" << x << ", y:" << y << ", tile: " << tile << "\n";

    // if there is no ball on the next tile, we have nothing to do
    if(Algorithms::noneOf(tile, Tileset::ball, Tileset::ballRed, Tileset::ballBlue) || direction.isNull()) {
        return false;
    }

    //std::cout << "Level1::isBlockingTile " << (int)tile << ", " << (int)Tileset::ball
    //              << ", " << (int)Tileset::ballRed << ", " << (int)Tileset::ballBlue << "\n";

    // we have a ball in front of the knight
    if(direction.x < -.5) {
        --x;
    } else if(direction.x > .5) {
        ++x;
    } else if(direction.y < -.5) {
        --y;
    } else if(direction.y > .5) {
        ++y;
    }

    //std::cout << "Level1::isBlockingTile new position: " << x << ", " << y << "\n";

    wchar_t nextTile = tileContent(x, y);
    //std::cout << "Level1::isBlockingTile new tile " << (int)nextTile << ", " << (int)Tileset::ball
    //              << ", " << (int)Tileset::ballRed << ", " << (int)Tileset::ballBlue << "\n";

    // there must not be any obstacle behind the ball what the knight will push
    if(Algorithms::anyOf(nextTile, Tileset::wall, Tileset::cactus1, Tileset::cactus2, Tileset::invalidField,
                         Tileset::palmTree1, Tileset::palmTree2, Tileset::ball, Tileset::ballRed, Tileset::ballBlue,
                         Tileset::staticStone, Tileset::building)) {
        return true;
    }

    //std::cout << "Tileset::isBlockingTile\n";
    // restrictions related to ball and hole colors
    if(tile == Tileset::ball && Algorithms::anyOf(nextTile, Tileset::holeRed, Tileset::holeBlue)) return true;
    if(tile == Tileset::ballRed && Algorithms::anyOf(nextTile, Tileset::hole, Tileset::holeBlue)) return true;
    //if(tile == Tileset::ballBlue && Algorithms::anyOf(nextTile, Tileset::hole, Tileset::holeRed)) return true;

    // there must not be another knight behind the ball
    return isTileOccupiedByAKnight(x, y);
}

bool Tileset::isTileOccupiedByAKnight(int x, int y) const noexcept {
    auto& world = Application::latestWorld();
    for(auto& knightP : world->knightList()) {
        auto& k = knightP.second;
        if(x == convertKnightX2TileX(k->x()) && y == convertKnightY2TileY(k->y())) return true;
    }
    return false;
}

void Tileset::removeItem(uint32_t id) {
    //std::cout << "Tileset::removeItem: " << id << "\n";
    auto it = std::find_if(items_.begin(), items_.end(), [id](const TilesetItemInfo& i){return i.gameObjectId == id;});
    tileMap_[width_ * it->tilePosition.y + it->tilePosition.x] = emptyField;
    items_.erase(it);

    auto it2 = std::find_if(stoneIds_.begin(), stoneIds_.end(), [id](const uint32_t& i){return i == id;});
    if(it2 != stoneIds_.end()) stoneIds_.erase(it2);

    auto it3 = std::find_if(holeIds_.begin(), holeIds_.end(), [id](const uint32_t& i){return i == id;});
    if(it3 != holeIds_.end()) holeIds_.erase(it3);
}

void Tileset::updateStonePosition(uint32_t id, int x, int y, RollingStoneInterface::Color color) {
    auto it = std::find_if(items_.begin(), items_.end(), [id](const TilesetItemInfo& i){return i.gameObjectId == id;});
    //std::cout << "tileMap content: " << tileMap_[width_ * it->tilePosition.y + it->tilePosition.x] << "\n";
    tileMap_[width_ * it->tilePosition.y + it->tilePosition.x] = emptyField;

    //std::cout << "Old position: " << it->tilePosition.x << ", " << it->tilePosition.y << "\n";
    it->tilePosition.x = convertStoneX2TileX(x);
    it->tilePosition.y = convertStoneY2TileY(y);

    switch(color) {
    case RollingStoneInterface::Color::Gray:
        tileMap_[width_ * it->tilePosition.y + it->tilePosition.x] = Tileset::ball;
        //std::cout << "Tileset::updateStonePosition: Tileset::ball\n";
        break;
    case RollingStoneInterface::Color::Red:
        tileMap_[width_ * it->tilePosition.y + it->tilePosition.x] = Tileset::ballRed;
        //std::cout << "Tileset::updateStonePosition: Tileset::ballRed\n";
        break;
    case RollingStoneInterface::Color::Blue:
        tileMap_[width_ * it->tilePosition.y + it->tilePosition.x] = Tileset::ballBlue;
        //std::cout << "Tileset::updateStonePosition: Tileset::ballBlue\n";
        break;
    }
    //std::cout << "New position: " << it->tilePosition.x << ", " << it->tilePosition.y << "\n";
}

void Tileset::setupStonHoleCollision(WorldUPtr& world) noexcept {
    // Which stone with which hole?
    for(auto stoneId : stoneIds_) {
        auto itStone = world->rollingStoneList().find(stoneId);
        for(auto holeId : holeIds_) {
            auto itHole = world->rpgBgElementList().find(holeId);
            Rect rect = itHole->second->collisionData()->rect();
            rect.x += itHole->second->x();
            rect.y += itHole->second->y();
            itStone->second->addHoleRect(rect);
        }
    }
}

const Maze::MazeVector& Tileset::getMazeVector(const Knight* knight, int heroX, int heroY) {
    mazeVector_ = Maze::MazeVector(height_, std::vector<char>(width_));
    int col = 0;
    int row = 0;
    for(const auto& tile : tileMap_) {
        if(Algorithms::anyOf(tile, Tileset::wall, Tileset::hole, Tileset::holeRed, Tileset::holeBlue, Tileset::cactus1, Tileset::cactus2,
                             Tileset::invalidField, Tileset::palmTree1, Tileset::palmTree2, Tileset::staticStone,
                             Tileset::building, Tileset::ball, Tileset::ballRed, Tileset::ballBlue)) {
            mazeVector_[row][col] = Tileset::wall; // blocking field from walking point of view
            ++col;
        } else if(Algorithms::anyOf(tile, Tileset::redKnight, Tileset::darkKnight, Tileset::blondeFemaleKnight, Tileset::greenFemaleKnight, Tileset::emptyField,
                                    Tileset::skull, Tileset::bush1, Tileset::bush2, Tileset::bush3, Tileset::lake, Tileset::boat)) {
            mazeVector_[row][col] = Tileset::emptyField; // hero or empty field
            ++col;
        } else if(tile == nextLine) {
            col = 0;
            ++row;
        }
    }
    // all of the PASSIVE knights should appear as wall in this vector
    auto& world = Application::latestWorld();
    for(auto& knightP : world->knightList()) {
        auto& k = knightP.second;
        if(k->id() == knight->id()) continue;
        col = convertKnightX2TileX(k->x());
        row = convertKnightY2TileY(k->y());
        mazeVector_[row][col] = Tileset::wall;
    }

    // The knight's type doesn't matter for A*, it uses red knight for search
    mazeVector_[heroY][heroX] = Tileset::redKnight;
    return mazeVector_;
}

// This calculation is for a level, where the puzzle should be centered or in the top left corner, if it's bigger then the screen
void Tileset::calcTileMapRect() {
    width_ = tileMap_.find(nextLine) + 1;
    height_ = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [this](const wchar_t& c){if(c == nextLine) ++height_;});
    // with_ - 1, because '\n' is counted in the with, but it is not visible
    position_.x = std::max(((int)screenWidth() - widthInPixels()) / 2, 20_ZRW);
    position_.y = std::max(((int)screenHeight() - heightInPixels()) / 2, 25_ZRH);
    //std::cout << "Tileset::calcSizeByTileMap: screenWidth(): " << screenWidth() << ", width: " << (width_ - 1) * tileSize_.width
    //          << ", 20_ZRW: " << 20_ZRW << "\n";
}

void Tileset::createRollingStone(WorldUPtr& world, RollingStoneInterface::Color color, int row, int col) {
    RollingStoneUPtr stone = std::make_unique<RollingStone>(0._ZRW, 0._ZRH, GameObject::Type::GOT_ROLLINGSTONE, color);
    stone->setX(position_.x + col * tileSize_.width + stoneV_.x);
    stone->setY(position_.y + row * tileSize_.height + stoneV_.y);
    Rect colliderRect(0_ZRD, 15_ZRD, 32_ZRD, 12_ZRD);
    CollisionData* collisionData = new CollisionData(colliderRect);
    stone->setCollisionData(collisionData);
    stone->add(Application::environment());

    // Which stone with which hole?
    // First stone into the first hole
    stoneIds_.insert(stone->id());
    items_.emplace_back(TilesetItemInfo{stone->id(), PositionI{col, row}, static_cast<BitmapId>(stone->skinMin())});
    world->addRollingStone(std::move(stone));
}

void Tileset::createHole(WorldUPtr& world, BitmapId bitmapId, int row, int col) {
    RPGBgElementUPtr holeElement = std::make_unique<RPGBgElement>(173_ZRW, 158_ZRH, bitmapId);
    CollisionData* collisionData = new CollisionData(Rect(0_ZRD, 0_ZRD, 30_ZRD, 12_ZRD));
    holeElement->setCollisionData(collisionData);
    holeElement->setX(position_.x + col * tileSize_.width);
    holeElement->setY(position_.y + row * tileSize_.height + 10_ZRD);
    holeIds_.insert(holeElement->id());
    items_.emplace_back(TilesetItemInfo{holeElement->id(), PositionI{col, row}, static_cast<BitmapId>(holeElement->skinMin())});
    world->addRPGBgElement(std::move(holeElement));
}

void Tileset::createKnight(WorldUPtr& world, GameObject::Type type, int row, int col) {
    KnightUPtr hero = KnightFactory::create(type);
    hero->setGravity(Application::environment()->gravity());
    hero->setX(position_.x + col * tileSize_.width + knightV_.x);
    hero->setY(position_.y + row * tileSize_.height + knightV_.y);
    world->addKnight(std::move(hero));
}

void Tileset::followTheKnight() noexcept {
    using namespace Application;
    auto& knights = latestWorld()->knightList();
    if(knights.size() == 1) {
        screen()->follow(knights.begin()->second.get(), environment()->gameMap());
    }
}

uint32_t Tileset::gameObjectId(int tileX, int tileY) const noexcept {
    for(auto& info : items_) {
        if(info.tilePosition.x == tileX && info.tilePosition.y == tileY) {
            return info.gameObjectId;
        }
    }
    return 0xffffffff;
}

void Tileset::incStonePushCounter() noexcept {
    ++stonePushCounter_;
    debug("Tileset::incStonePushCounter stonePushCounter: %d", stonePushCounter_);
}

void Tileset::updateHistory(Knight* knight, MoveHistory::EventType eventType, VectorD direction) noexcept {
    if(eventType == MoveHistory::EventType::MOVE &&
       std::find_if(moveHistory_.events().begin(), moveHistory_.events().end(),
                    [](const auto& item) {return item.type == MoveHistory::EventType::MOVE;}) != moveHistory_.events().end()) {
        return;
    }
    auto tileX = convertKnightX2TileX(knight->x());
    auto tileY = convertKnightY2TileY(knight->y());
    if(eventType == MoveHistory::EventType::UNDO_POINT) {
        for(auto eventIt = moveHistory_.events().rbegin(); eventIt != moveHistory_.events().rend(); ++eventIt) {
            auto& lastEvent = *eventIt;
            if(lastEvent.type != MoveHistory::EventType::UNDO_POINT) continue;

            auto prevTileX = convertKnightX2TileX(lastEvent.position.x);
            auto prevTileY = convertKnightY2TileY(lastEvent.position.y);
            if(prevTileX == tileX && prevTileY == tileY) return;

            // Only the closest undo point has to be checked
            break;
        }
    }
    std::vector<uint8_t> cardsAmount;
    for(auto& item : Application::latestWorld()->otherElementList()) {
        if(Card* card = item.second->card()) {
            cardsAmount.push_back(card->amount());
        }
    }
    std::map<uint32_t, PositionD> darkKnights; // dark knights
    for(auto& item : Application::latestWorld()->knightList()) {
        if(item.second->type() == GameObjectInterface::Type::GOT_KNIGHT_DARK) {
            darkKnights.emplace(item.second->id(), PositionD{item.second->x(), item.second->y()});
        }
    }

    MoveHistory::Event event{
        knight->id(),
        Position<double>{convertTileX2KnightX(tileX), convertTileY2KnightY(tileY)},
        eventType,
        direction,
        tileMap_,
        items_,
        holeIds_,
        stoneIds_,
        stonePushCounter_,
        position_,
        cardsAmount,
        darkKnights
    };

    moveHistory_.addEvent(event);
}

void Tileset::restoreMembersFromHistoryEvent(const MoveHistory::Event& lastEvent) noexcept {
    // restore stones
    stoneIds_ = lastEvent.stoneIds;

    // restore hero2's position

    // restore holes
    holeIds_ = lastEvent.holeIds;

    // restore tileMap_
    tileMap_ = lastEvent.tileMap;

    // restore items_
    items_ = lastEvent.items;

    // restore position
    position_ = lastEvent.tilesetPosition;
}

void Tileset::syncHoleIdsAndHoleObjects() noexcept {
    using namespace Application;
    std::vector<uint32_t> idsToErase;
    for(auto& id : holeIds_) {
        auto it = std::find_if(items_.begin(), items_.end(), [id](const TilesetItemInfo& info) -> bool {return info.gameObjectId == id;});
        assert(it != items_.end());

        auto rpgIt = latestWorld()->rpgBgElementList().find(id);
        if(rpgIt == latestWorld()->rpgBgElementList().end()) {
            int holePositionX = it->tilePosition.x;
            int holePositionY = it->tilePosition.y;
            items_.erase(it);
            idsToErase.push_back(id);
            createHole(latestWorld(), NATIVE_BITMAP_HOLE_DARK_01, holePositionY, holePositionX);
        } else {
            rpgIt->second->setX(position_.x + it->tilePosition.x * tileSize_.width);
            rpgIt->second->setY(position_.y + it->tilePosition.y * tileSize_.height + 10_ZRD);
        }
    }
    auto rIt = items_.rbegin();
    for(auto id : idsToErase) {
        holeIds_.erase(id);
        moveHistory_.replaceItemId(id, rIt->gameObjectId, MoveHistory::ReplaceType::HOLE);
        ++rIt;
    }
}

void Tileset::syncStoneIdsAndStoneObjects() noexcept {
    using namespace Application;
    std::vector<uint32_t> idsToErase;
    for(auto& id : stoneIds_) {
        auto it = std::find_if(items_.begin(), items_.end(), [id](const TilesetItemInfo& info) -> bool {return info.gameObjectId == id;});
        assert(it != items_.end());

        auto rpgIt = latestWorld()->rollingStoneList().find(id);
        if(rpgIt == latestWorld()->rollingStoneList().end()) {
            int stonePositionX = it->tilePosition.x;
            int stonePositionY = it->tilePosition.y;
            items_.erase(it);
            idsToErase.push_back(id);
            createRollingStone(latestWorld(), RollingStoneInterface::Color::Gray, stonePositionY, stonePositionX);
        } else {
            rpgIt->second->setX(convertTileX2StoneX(it->tilePosition.x));
            rpgIt->second->setY(convertTileY2StoneY(it->tilePosition.y));
        }
    }

    auto rIt = items_.rbegin();
    for(auto id : idsToErase) {
        stoneIds_.erase(id);
        moveHistory_.replaceItemId(id, rIt->gameObjectId, MoveHistory::ReplaceType::STONE);
        auto itStone = latestWorld()->rollingStoneList().find(rIt->gameObjectId);
        for(auto holeId : holeIds_) {
            auto itHole = latestWorld()->rpgBgElementList().find(holeId);
            Rect rect = itHole->second->collisionData()->rect();
            rect.x += itHole->second->x();
            rect.y += itHole->second->y();
            itStone->second->addHoleRect(rect);
        }
        ++rIt;
    }
}

void Tileset::restoreCardsFromHistoryEvent(const MoveHistory::Event& lastEvent) noexcept {
    using namespace Application;
    int cardAmountIndex = 0;
    for(auto& item : latestWorld()->otherElementList()) {
        if(Card* card = item.second->card()) {
            card->setAmount(lastEvent.cardsAmount[cardAmountIndex++]);
        }
    }
}

void Tileset::restoreDarkKnightsPositionFromHistoryEvent(const MoveHistory::Event& lastEvent) noexcept {
    using namespace Application;
    for(auto& item : lastEvent.darkKnights) {
        auto it = latestWorld()->knightList().find(item.first);
        it->second->setX(item.second.x);
        it->second->setY(item.second.y);
    }
}

void Tileset::undoLastStep(const MoveHistory::Event& lastEvent, const KnightUPtr& knight) noexcept {
    using namespace Application;

    // restore knight's position
    knight->setX(lastEvent.position.x);
    knight->setY(lastEvent.position.y);

    restoreMembersFromHistoryEvent(lastEvent);
    syncHoleIdsAndHoleObjects();
    syncStoneIdsAndStoneObjects();

    stonePushCounter_ = lastEvent.stonePushCounter;

    restoreCardsFromHistoryEvent(lastEvent);
    restoreDarkKnightsPositionFromHistoryEvent(lastEvent);
}

void Tileset::undoLastStep() noexcept {
    using namespace Application;
    if(environment()->activeKnightId() == Environment::NONE_ACTIVE_KNIGHT) return;
    auto it = latestWorld()->knightList().find(environment()->activeKnightId());
    if(it == latestWorld()->knightList().end()) return;
    if(it->second->isRunning()) {
        debug("WARNING: you want undo but the hero is running");
        return;
    }
    if(!isUndoCompatible()) return;

    const KnightUPtr& knight = it->second;

    size_t eventsToDelete = 0;
    for(auto eventIt = moveHistory_.events().rbegin(); eventIt != moveHistory_.events().rend(); ++eventIt) {
        ++eventsToDelete;
        auto& lastEvent = *eventIt;
        if(lastEvent.type != MoveHistory::EventType::UNDO_POINT) continue;
        if(lastEvent.gameObjectId != knight->id()) return;

        undoLastStep(lastEvent, knight);

        for(size_t i = 0; i < eventsToDelete; ++i) moveHistory_.dropLastEvent();
        break;
    }
}

void Tileset::update(double now) noexcept {
    updateTexts(now);
    updateElapsedTime(now);
    teamCountCheck();
    updateTeamMarker(now);
}

void Tileset::drawTexts(int levelIndex, int minimumPushCount) noexcept {
    auto& f16px = Application::font();
    (*f16px).setColor(200, 50, 50);
    std::wstring txt = (*Application::translator())(TranslateInterface::Key::TRA__LEVEL);
    StringUtil::wstr_replace(txt, L"%d", std::to_wstring(levelIndex));
    (*f16px).setBold(true);
    (*f16px).draw(Application::screen(), screenPosX() + screenWidth() - 75_ZRD, screenPosY() + 2_ZRD, txt);
    (*f16px).setBold(false);

    auto& f12px = Application::font12px();
    (*f12px).setColor(50, 50, 50);
    txt = (*Application::translator())(TranslateInterface::Key::TRA__PUSHES) + L": " + std::to_wstring(stonePushCounter_) + L"/" + std::to_wstring(minimumPushCount);
    (*f12px).draw(Application::screen(), screenPosX() + screenWidth() - 75_ZRD, screenPosY() + 18_ZRD, txt);

}
