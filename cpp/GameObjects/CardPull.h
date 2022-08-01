#ifndef __CARDPULL_H__
#define __CARDPULL_H__

#include "Card.h"

class CardPull : public Card
{
public:
    CardPull(double coolDownTime) noexcept;
    BlueTriangle* copy() const noexcept override;


    // from BlueTriangle
    const CardPull* cardPull() const noexcept override {return this;}
    CardPull* cardPull() noexcept override {return this;}

    void setActive(bool active) noexcept override;
    void setBitmapDisabled() noexcept override;
    void setBitmapNormal() noexcept override;
};

#endif // __CARDPULL_H__
