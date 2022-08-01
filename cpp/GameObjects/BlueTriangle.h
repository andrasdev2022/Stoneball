#ifndef __BLUETRIANGLE_H__
#define __BLUETRIANGLE_H__

#include "GameObject.h"
#include "GameObjectDescriptor.h"

class RedBouncingStone;
class GrayBouncingStone;
class YellowBouncingStone;
class GreenBouncingStone;
class Messagebox1;
class Listbox1;
class Selectionbox;
class DeckOfCards;
class CardPull;
class CardPushPlayer;
class CardTeleport;
class Card;
class Questionmark;
class Slider;
class MatchResults;
class MessageboxYesNo;
class ObserverTouchEvent;
class Fireworks;
class Trophy;
class EmoteFigure;
class MessageboxTermsOfUse;

class BlueTriangle : public GameObject {
public:
    BlueTriangle(double x, double bottomY, GameObject::Type type = GameObject::Type::GOT_BLUE_TRIANGLE) noexcept;
    BlueTriangle(const BlueTriangle& g) = default;

    // GameObject
    const GameObjectDescriptor& god() const noexcept override;
    void update(double) noexcept override;
    double z() const noexcept override;
    void die() noexcept override {dead_ = true;}
    bool isDead() const noexcept override {return dead_;}
    virtual bool flipped() const noexcept override {return flipped_;}
    virtual void setFlipped(bool flipped) noexcept override {flipped_ = flipped;}

    virtual BlueTriangle* copy() const noexcept;

    std::string name() const noexcept {return name_;}
    void setName(std::string name) {name_ = name;}

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

    // dynamic_cast
    virtual BlueTriangle* blueTriangle() noexcept override {return this;}
    virtual RedBouncingStone* redBouncingStone() noexcept {return nullptr;}
    virtual GrayBouncingStone* grayBouncingStone() noexcept {return nullptr;}
    virtual YellowBouncingStone* yellowBouncingStone() noexcept {return nullptr;}
    virtual GreenBouncingStone* greenBouncingStone() noexcept {return nullptr;}
    virtual Messagebox1* messagebox1() noexcept {return nullptr;}
    virtual const Messagebox1* messagebox1() const noexcept {return nullptr;}
    virtual Listbox1* listbox1() noexcept {return nullptr;}
    virtual Selectionbox* selectionbox() noexcept {return nullptr;}
    virtual DeckOfCards* deckOfCards() noexcept {return nullptr;}
    virtual const CardPull* cardPull() const noexcept {return nullptr;}
    virtual CardPull* cardPull() noexcept {return nullptr;}
    virtual const CardPushPlayer* cardPushPlayer() const noexcept {return nullptr;}
    virtual CardPushPlayer* cardPushPlayer() noexcept {return nullptr;}
    virtual const CardTeleport* cardTeleport() const noexcept {return nullptr;}
    virtual CardTeleport* cardTeleport() noexcept {return nullptr;}
    virtual const Card* card() const noexcept {return nullptr;}
    virtual Card* card() noexcept {return nullptr;}
    virtual Questionmark* questionmark() noexcept {return nullptr;}
    virtual Slider* slider() noexcept {return nullptr;}
    virtual MatchResults* matchResults() noexcept {return nullptr;}
    virtual const MatchResults* matchResults() const noexcept {return nullptr;}
    virtual MessageboxYesNo* messageboxYesNo() noexcept {return nullptr;}
    virtual const MessageboxYesNo* messageboxYesNo() const noexcept {return nullptr;}
    virtual ObserverTouchEvent* observerTouchEvent() noexcept {return nullptr;}
    virtual Fireworks* fireworks() noexcept {return nullptr;}
    virtual Trophy* trophy() noexcept {return nullptr;}
    virtual EmoteFigure* emoteFigure() noexcept {return nullptr;}
    virtual MessageboxTermsOfUse* messageboxTermsOfUse() noexcept {return nullptr;}
    virtual const MessageboxTermsOfUse* messageboxTermsOfUse() const noexcept {return nullptr;}

private:
    bool dead_{false};
    bool flipped_{false};
    std::string name_;
};

using BlueTriangleUPtr = std::unique_ptr<BlueTriangle>;
#endif // __BLUETRIANGLE_H__
