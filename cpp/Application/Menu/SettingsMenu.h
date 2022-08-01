#ifndef __SETTINGSMENU_H__
#define __SETTINGSMENU_H__

#include "ApplicationMenu.h"
#include "Rect.h"

class Selectionbox;
class Slider;

class SettingsMenu : public ApplicationMenu
{
public:
    SettingsMenu();

    void update(double) noexcept override;
    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;
    SettingsMenu* settingsMenu() noexcept override {return this;}

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

    virtual void initSelectionboxForLanguages() noexcept;
#if !defined __SERVERAPP__ && !defined __TEST__ && !defined ANDROID_NDK
    virtual void initSelectionboxForResolution() noexcept; // only the desktop version implements it
#endif
    virtual void initSlidersForAudioControl() noexcept;

    virtual void musicVolumeValueChangedCallback(Slider* slider, int value) noexcept;
    virtual void effectVolumeValueChangedCallback(Slider* slider, int value) noexcept;

    virtual void resetVolumes() noexcept;

private:
    Selectionbox* selectionBox_{};
    Slider* musicSlider_{};
    Slider* effectSlider_{};
    int musicVolumeValue_{100};
    int effectVolumeValue_{100};

#if !defined __SERVERAPP__ && !defined __TEST__ && !defined ANDROID_NDK
    Selectionbox* resolutionSelectionBox_{};
    int resolutionIndex_{};
#endif

    bool offsetsInitialized_{};
    Rect saveButtonRect_;
};

#endif // __SETTINGSMENU_H__
