#ifndef __ANIMATEDKNIGHTSFORMENU_H__
#define __ANIMATEDKNIGHTSFORMENU_H__

#if !defined(ANDROID_NDK)
class AndroidBitmapInfo;
#else
#include <android/bitmap.h>
#endif

#include <vector>
#include <memory>

class Knight;
using KnightSPtr = std::shared_ptr<Knight>;
class Selectionbox;
class ObserverTouchEventSubject;

class AnimatedKnightsForMenu
{
public:
    AnimatedKnightsForMenu();
    virtual ~AnimatedKnightsForMenu() = default;

    virtual void update(double now) noexcept;
    virtual bool draw(AndroidBitmapInfo* info, void* pixels) noexcept;
    virtual void sitDown() noexcept;
    virtual void clear() noexcept;
    virtual void init(int avatarNum) noexcept;
    virtual void add(KnightSPtr knight) noexcept;

    virtual void selectionboxCallbackForMaps(Selectionbox* selectionbox, int selectedLineNr)  noexcept;
    virtual void initSelectionboxForMaps() noexcept;
    virtual Selectionbox* selectionboxForMaps() noexcept {return selectionboxForMaps_;}

    virtual bool avatarSelectionCheck(ObserverTouchEventSubject* subject) noexcept;
    virtual bool avatarSelectionCheckSimple(ObserverTouchEventSubject* subject) noexcept;

    virtual int standingAvatar() noexcept;

    void initOffsets(int xOffset, int yOffset) noexcept;

private:
    std::vector<KnightSPtr> multiplayerAnimatedHeros_;
    Selectionbox* selectionboxForMaps_{};
    int lastSelectedLineNr_{};
    int xOffset_{};
    int yOffset_{};
    int offsetsState_{};
    bool selectionboxForMapOffsetUpdated_{};
};

using AnimatedKnightsForMenuUPtr = std::unique_ptr<AnimatedKnightsForMenu>;

#endif // __ANIMATEDKNIGHTSFORMENU_H__
