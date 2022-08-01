#ifndef __TILESETCARDHANDLER_H__
#define __TILESETCARDHANDLER_H__

#include <stdint.h>
#include <vector>
#include <memory>

class World;
using WorldUPtr = std::unique_ptr<World>;

class CardPull;
class CardPushPlayer;
class CardTeleport;

class TilesetCardHandler {
public:
    TilesetCardHandler(int teamCount);
    virtual ~TilesetCardHandler() = default;

    virtual void updateDecks(double now) noexcept;
    virtual void updateCards(double now) noexcept;

    virtual void toggleCardsVisibility(uint32_t deckId) noexcept;
    virtual void hideAllCards() noexcept;
    [[nodiscard]] virtual bool anyCardVisible() const noexcept;
    [[nodiscard]] virtual bool isPullCardActive() const noexcept;
    [[nodiscard]] virtual bool isPushPlayerCardActive() const noexcept;
    [[nodiscard]] virtual bool isTeleportCardActive() const noexcept;
    [[nodiscard]] virtual uint32_t deckId(int team) const noexcept {return deckIds_[team];}
    [[nodiscard]] virtual uint32_t& deckId(int team) noexcept {return deckIds_[team];}
    [[nodiscard]] virtual CardPull* getActivePullCard() const noexcept;
    [[nodiscard]] virtual CardPushPlayer* getActivePushPlayerCard() const noexcept;
    [[nodiscard]] virtual CardTeleport* getActiveTeleportCard() const noexcept;
    [[nodiscard]] virtual std::vector<uint32_t>& availableCardIds(int team) noexcept {return availableCardIds_[team];}

protected:
    virtual void initDecks(WorldUPtr& world, int pullAmount, int pushAmount, int teleportAmount) noexcept;
    virtual void initCards(WorldUPtr& world, int pullAmount, int pushAmount, int teleportAmount) noexcept;
    virtual void setDeckId(int team, uint32_t id) noexcept {deckIds_[team] = id;}
    [[nodiscard]] virtual int textHeight() const noexcept {return textHeight_;}
    [[nodiscard]] virtual int textXPos(int teamIndex) const noexcept {return textXPos_[teamIndex];}
    [[nodiscard]] virtual int textYPos() const noexcept {return textYPos_;}
    virtual void updateMinimumTextXDistance() noexcept;

    int textHeight_{};
    std::vector<int> textXPos_;
    int textYPos_{};

private:
    std::vector<uint32_t> deckIds_;
    std::vector<std::vector<uint32_t>> availableCardIds_; // teamIndex, key: card id
};

#endif // __TILESETCARDHANDLERINTERFACE_H__
