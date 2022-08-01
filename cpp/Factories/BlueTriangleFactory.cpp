#include "BlueTriangleFactory.h"
#include "TranslateInterface.h"
#include "BouncingStones.h"
#include "Messagebox1.h"
#include "Listbox1.h"
#include "DeckOfCards.h"
#include "CardPull.h"
#include "CardPushPlayer.h"
#include "CardTeleport.h"
#include "Questionmark.h"
#include "Slider.h"
#include "MatchResults.h"
#include "MessageboxYesNo.h"
#include "MessageboxTermsOfUse.h"
#include "Fireworks.h"
#include "Trophy.h"
#include "EmoteFigure.h"

extern double coolDownPullCard;
extern double coolDownPushPlayerCard;
extern double coolDownTeleportCard;

BlueTriangleUPtr BlueTriangleFactory::create(GameObject::Type type) noexcept {
    BlueTriangleUPtr otherElement;
    switch(type) {
    case GameObject::Type::GOT_BLUE_TRIANGLE: otherElement.reset(new BlueTriangle(0, 0, GameObject::Type::GOT_BLUE_TRIANGLE));
         break;
    case GameObject::Type::GOT_RED_BOUNCING_STONE: otherElement.reset(new RedBouncingStone(0, 0));
         break;
    case GameObject::Type::GOT_GRAY_BOUNCING_STONE: otherElement.reset(new GrayBouncingStone(0, 0));
         break;
    case GameObject::Type::GOT_YELLOW_BOUNCING_STONE: otherElement.reset(new YellowBouncingStone(0, 0));
         break;
    case GameObject::Type::GOT_GREEN_BOUNCING_STONE: otherElement.reset(new GreenBouncingStone(0, 0));
         break;
    case GameObject::Type::GOT_MESSAGEBOX: otherElement.reset(new Messagebox1(0, TranslateInterface::Key::LAST_ONE));
         break;
    case GameObject::Type::GOT_LISTBOX: otherElement.reset(new Listbox1(0, 0, 0));
         break;
    case GameObject::Type::GOT_DECK_OF_CARDS: otherElement.reset(new DeckOfCards(0, 0));
        break;
    case GameObject::Type::GOT_CARD_PULL: otherElement.reset(new CardPull(coolDownPullCard));
        break;
    case GameObject::Type::GOT_CARD_PUSH_PLAYER: otherElement.reset(new CardPushPlayer(coolDownPushPlayerCard));
        break;
    case GameObject::Type::GOT_CARD_TELEPORT: otherElement.reset(new CardTeleport(coolDownTeleportCard));
        break;
    case GameObject::Type::GOT_QUESTIONMARK: otherElement.reset(new Questionmark(0, 0, 0));
        break;
    case GameObject::Type::GOT_SLIDER: otherElement.reset(new Slider(0, 0, 0, 1));
        break;
    case GameObject::Type::GOT_MATCH_RESULTS: otherElement.reset(new MatchResults(0));
        break;
    case GameObject::Type::GOT_MESSAGEBOXYESNO: otherElement.reset(new MessageboxYesNo());
        break;
    case GameObject::Type::GOT_FIREWORK_LONG_BLUE:
    case GameObject::Type::GOT_FIREWORK_LONG_ORANGE:
    case GameObject::Type::GOT_FIREWORK_LONG_GREEN: otherElement.reset(new Fireworks(0, 0, type));
        break;
    case GameObject::Type::GOT_TROPHY_NR1:
    case GameObject::Type::GOT_TROPHY_NR2:
    case GameObject::Type::GOT_TROPHY_NR3: otherElement.reset(new Trophy(0, 0, type));
        break;
    case GameObject::Type::GOT_EMOTE_FIGURE: otherElement.reset(new EmoteFigure(0, 0, 0, 0));
        break;
    case GameObject::Type::GOT_MESSAGEBOXTERMSOFUSE: otherElement.reset(new MessageboxTermsOfUse());
        break;
    default:
        debug("ERROR: BlueTriangleFactory::create");
        break;
    }
    return otherElement;
}
