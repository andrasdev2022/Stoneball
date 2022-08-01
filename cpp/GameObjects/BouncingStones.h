#ifndef __BOUNCINGSTONES_H__
#define __BOUNCINGSTONES_H__

#include "BlueTriangle.h"

class RedBouncingStone : public BlueTriangle {
public:
    RedBouncingStone(double x, double bottomY) noexcept;
    BlueTriangle* copy() const noexcept override;
    RedBouncingStone* redBouncingStone() noexcept override {return this;}
};

class GrayBouncingStone : public BlueTriangle {
public:
    GrayBouncingStone(double x, double bottomY) noexcept;
    BlueTriangle* copy() const noexcept override;
    GrayBouncingStone* grayBouncingStone() noexcept override {return this;}
};

class YellowBouncingStone : public BlueTriangle {
public:
    YellowBouncingStone(double x, double bottomY) noexcept;
    BlueTriangle* copy() const noexcept override;
    YellowBouncingStone* yellowBouncingStone() noexcept override {return this;}
};

class GreenBouncingStone : public BlueTriangle {
public:
    GreenBouncingStone(double x, double bottomY) noexcept;
    BlueTriangle* copy() const noexcept override;
    GreenBouncingStone* greenBouncingStone() noexcept override {return this;}
};

#endif // __REDBOUNCINGSTONES_H__
