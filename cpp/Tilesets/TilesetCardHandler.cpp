#include "TilesetCardHandler.h"
#include "Application.h"
#include "World.h"
#include "CardPull.h"
#include "CardPushPlayer.h"
#include "CardTeleport.h"
#include "Tileset.h"
#include "DeckOfCards.h"
#include "MediaData.h"

using namespace Application;

TilesetCardHandler::TilesetCardHandler(int teamCount)
: textXPos_(teamCount)
, deckIds_(teamCount)
, availableCardIds_(teamCount) {
    for(size_t team = 0; team != deckIds_.size(); ++team) {
        deckIds_[team] = 0xffffffff;
    }
}
void TilesetCardHandler::hideAllCards() noexcept {
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->card()) {
            it->second->card()->hide();
        }
    }
}

// multiplayer client calls it, where availableCardIds_ is empty
bool TilesetCardHandler::anyCardVisible() const noexcept {
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->card() && it->second->card()->visible()) {
            return true;
        }
    }
    return false;
}

bool TilesetCardHandler::isPullCardActive() const noexcept {
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->cardPull() && it->second->cardPull()->active() &&
        (!latestWorld()->multiplayer() || it->second->cardPull()->teamIndex() == Application::tileset()->activeTeamId())) {
            return true;
        }
    }
    return false;
}

bool TilesetCardHandler::isPushPlayerCardActive() const noexcept {
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->cardPushPlayer() && it->second->cardPushPlayer()->active() &&
        (!latestWorld()->multiplayer() || it->second->cardPushPlayer()->teamIndex() == Application::tileset()->activeTeamId())) {
            return true;
        }
    }
    return false;
}

bool TilesetCardHandler::isTeleportCardActive() const noexcept {
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->cardTeleport() && it->second->cardTeleport()->active() &&
        (!latestWorld()->multiplayer() || it->second->cardTeleport()->teamIndex() == Application::tileset()->activeTeamId())) {
            return true;
        }
    }
    return false;
}

CardPull* TilesetCardHandler::getActivePullCard() const noexcept {
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->cardPull() && it->second->cardPull()->active() &&
        (!latestWorld()->multiplayer() || it->second->cardPull()->teamIndex() == Application::tileset()->activeTeamId())) {
            return it->second->cardPull();
        }
    }
    return nullptr;
}

CardPushPlayer* TilesetCardHandler::getActivePushPlayerCard() const noexcept {
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->cardPushPlayer() && it->second->cardPushPlayer()->active() &&
        (!latestWorld()->multiplayer() || it->second->cardPushPlayer()->teamIndex() == Application::tileset()->activeTeamId())) {
            return it->second->cardPushPlayer();
        }
    }
    return nullptr;
}

CardTeleport* TilesetCardHandler::getActiveTeleportCard() const noexcept {
    /*
    for(auto it = latestWorld()->otherElementList().begin(); it != latestWorld()->otherElementList().end(); ++it) {
        if(it->second->cardTeleport() && it->second->cardTeleport()->active() && it->second->cardTeleport()->teamIndex() == Application::tileset()->activeTeamId()) {
            return it->second->cardTeleport();
        }
    }
    */
    for(auto& [id, element] : latestWorld()->otherElementList()) {
        Unused(0, id);
        if(element->cardTeleport() && element->cardTeleport()->active() &&
        (!latestWorld()->multiplayer() || element->cardTeleport()->teamIndex() == Application::tileset()->activeTeamId())) {
            return element->cardTeleport();
        }
    }
    return nullptr;
}

void TilesetCardHandler::toggleCardsVisibility(uint32_t deckId) noexcept {
    auto it = std::find(deckIds_.begin(), deckIds_.end(), deckId);
    if(it == deckIds_.end()) return;

    for(auto& card : availableCardIds_[std::distance(deckIds_.begin(), it)]) {
        auto it = latestWorld()->otherElementList().find(card);
        if(it != latestWorld()->otherElementList().end()) {
            if(it->second->card()) {
                if(it->second->card()->amount() == 0) continue;
                if(it->second->card()->visible()) {
                    it->second->card()->hide();
                } else {
                    it->second->card()->show();
                }
            }
        }
    }
}

void TilesetCardHandler::initDecks(WorldUPtr& world, int pullAmount, int pushAmount, int teleportAmount) noexcept {
    // add the deck of cards
    for(size_t team = 0; team != deckIds_.size(); ++team) {
        BlueTriangleUPtr deck(new DeckOfCards(0, 0));
        deckId(team) = deck->id();
        world->addOtherElement(std::move(deck));
    }
    initCards(world, pullAmount, pushAmount, teleportAmount);
}

double coolDownPullCard;
double coolDownPushPlayerCard;
double coolDownTeleportCard;

void TilesetCardHandler::initCards(WorldUPtr& world, int pullAmount, int pushAmount, int teleportAmount) noexcept {

    coolDownPullCard = Application::playTime() / 5. * 60.e3;
    coolDownPushPlayerCard = Application::playTime() / 5. * 120.e3;
    coolDownTeleportCard = Application::playTime() / 5. * 360.e3;
/*
    coolDownPullCard = 60.e3;
    coolDownPushPlayerCard = 120.e3;
    coolDownTeleportCard = 360.e3;
*/
    for(size_t team = 0; team != deckIds_.size(); ++team) {
        CardUPtr card(new CardPull(coolDownPullCard));
        card->setAmount(pullAmount);
        card->setTeamIndex(team);
        card->setCoolDown(0);
        availableCardIds(team).push_back(card->id());
        world->addOtherElement(std::move(card));

        card.reset(new CardPushPlayer(coolDownPushPlayerCard));
        card->setAmount(pushAmount);
        card->setTeamIndex(team);
        card->setCoolDown(0);
        availableCardIds(team).push_back(card->id());
        world->addOtherElement(std::move(card));

        card.reset(new CardTeleport(coolDownTeleportCard));
        card->setAmount(teleportAmount);
        card->setTeamIndex(team);
        card->setCoolDown(0);
        availableCardIds(team).push_back(card->id());
        world->addOtherElement(std::move(card));
    }
}

// multiplayer client must not run the updateDecks
void TilesetCardHandler::updateDecks([[maybe_unused]]double now) noexcept {
    auto& otherElementList = latestWorld()->otherElementList();
    for(size_t teamIndex = 0; teamIndex < deckIds_.size(); ++teamIndex) {
        auto it = otherElementList.find(deckId(teamIndex));
        if(it != otherElementList.end()) {
            it->second->setX(textXPos_[teamIndex] + 30_ZRD);
            it->second->setY(textYPos_ + textHeight_);
        }
    }
}

// multiplayer client must not run the updateCards
void TilesetCardHandler::updateCards([[maybe_unused]]double now) noexcept {
    for(size_t teamIndex = 0; teamIndex < deckIds_.size(); ++teamIndex) {
        auto& teamCards = availableCardIds(teamIndex);
        int cardIndex = 0;
        for(auto& card : teamCards) {
            auto it = latestWorld()->otherElementList().find(card);
            if(it != latestWorld()->otherElementList().end()) {
                if(it->second->card()) {
                    if(it->second->card()->amount() == 0) continue;
                    if(it->second->card()->visible()) {
                        it->second->setX(textXPos_[teamIndex] - 26_ZRD +  56_ZRD * cardIndex);
                        it->second->setY(textYPos_ + textHeight_ + 10_ZRD);
                        ++cardIndex;
                    }
                }
            }
        }
    }
}

void TilesetCardHandler::updateMinimumTextXDistance() noexcept {
    int diff;
    int oldOne;
    for(size_t i = 1; i < textXPos_.size(); ++i) {
        if(textXPos_[i] - textXPos_[i - 1] < 115_ZRD) {
            oldOne = textXPos_[i];
            textXPos_[i] = textXPos_[i - 1] + 115_ZRD;
            diff = textXPos_[i] - oldOne;
            for(size_t j = i + 1; j < textXPos_.size(); ++j) {
                textXPos_[j] += diff;
            }
        }
    }
}
