#ifndef __CARDTELEPORT_H__
#define __CARDTELEPORT_H__


#include "Card.h"

class CardTeleport : public Card
{
public:
    CardTeleport(double coolDownTime) noexcept;
    BlueTriangle* copy() const noexcept override;

    // from BlueTriangle
    virtual const CardTeleport* cardTeleport() const noexcept override {return this;}
    virtual CardTeleport* cardTeleport() noexcept override {return this;}

    void setActive(bool active) noexcept override;
    void setBitmapDisabled() noexcept override;
    void setBitmapNormal() noexcept override;
};

#endif // __CARDTELEPORT_H__
