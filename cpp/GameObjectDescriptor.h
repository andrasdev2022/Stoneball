#ifndef __GAMEOBJECTDESCRIPTOR_H__
#define __GAMEOBJECTDESCRIPTOR_H__

#include "GameObjectInterface.h"
#include "State.h"

class GameObjectDescriptor {
public:
    static const GameObjectDescriptor& find(GameObjectInterface::Type got, State::Type st) noexcept;

    int skinStart() const noexcept {return skinStart_;}
    void setSkinStart(int v) noexcept {skinStart_ = v;}
    int skinMin() const noexcept {return skinMin_;}
    void setSkinMin(int v) noexcept {skinMin_ = v;}
    int skinMax() const noexcept {return skinMax_;}
    void setSkinMax(int v) noexcept {skinMax_ = v;}
    double animTimeStep() const noexcept {return animTimeStep_;}
    void setAnimTimeStep(double v) noexcept {animTimeStep_ = v;}
    int yPosCorrection() const noexcept {return yPosCorrection_;}
    void setYPosCorrection(int v) noexcept {yPosCorrection_ = v;}
    double velocity() const noexcept {return velocity_;}
    void setVelocity(double v) noexcept {velocity_ = v;}

private:
    static void initDescriptor() noexcept;
    static void initButtonFrames() noexcept;
    static void initRedKnightFrames() noexcept;
    static void initDarkKnightFrames() noexcept;
    static void initBlondeFemaleKnightFrames() noexcept;
    static void initGreenFemaleKnightFrames() noexcept;
    static void initEnvironmentFrames() noexcept;
    static void initTriangle() noexcept;
    static void initBouncingStones() noexcept;
    static void initMessageboxes() noexcept;

    /*
    static void initBlueKnightFrames() noexcept;
    static void initWhiteKnightFrames() noexcept;
    static void initYellowKnightFrames() noexcept;
    static void initParticleFrames() noexcept;
    */
    int skinStart_;
    int skinMin_;
    int skinMax_;
    double animTimeStep_;
    int yPosCorrection_; // relative to the Knight position
    double velocity_;
};
#endif // __GAMEOBJECTDESCRIPTOR_H__
