#ifndef __TROPHY_H__
#define __TROPHY_H__

#include "BlueTriangle.h"

class Trophy : public BlueTriangle
{
public:
    Trophy(double x, double bottomY, GameObjectInterface::Type type) noexcept;

    BlueTriangle* copy() const noexcept override;

    // GameObject
    void update(double now) noexcept override;
    const GameObjectDescriptor& god() const noexcept override;
    double z() const noexcept override {return 10000.;}

    Trophy* trophy() noexcept override {return this;}
private:
    double time2Die_{};
};

using TrophyUPtr = std::unique_ptr<Trophy>;

#endif // __TROPHY_H__
