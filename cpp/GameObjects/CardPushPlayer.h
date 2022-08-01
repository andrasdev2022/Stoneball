#ifndef __CARDPUSHPLAYER_H__
#define __CARDPUSHPLAYER_H__


#include "Card.h"

class CardPushPlayer : public Card
{
public:
    CardPushPlayer(double coolDownTime) noexcept;
    BlueTriangle* copy() const noexcept override;

    // from BlueTriangle
    const CardPushPlayer* cardPushPlayer() const noexcept override {return this;}
    CardPushPlayer* cardPushPlayer() noexcept override {return this;}

    void setActive(bool active) noexcept override;
    void setBitmapDisabled() noexcept override;
    void setBitmapNormal() noexcept override;
};

#endif // __CARDPUSHPLAYER_H__
