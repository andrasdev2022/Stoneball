#ifndef __APPLICATIONEXPORTS_H__
#define __APPLICATIONEXPORTS_H__

#include "stoneball.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    void drawWorld(AndroidBitmapInfo* info, void* pixels);
    void updateWorld(double now); // Server calls it
    void onTouchEvent(int action, double x, double y, double now);
    void wakeUp();
    int musicVolume();
    int effectVolume();
    void setMusicVolume(int volume);
    void setEffectVolume(int volume);
    int* levelStatuses();
    bool hasSettingsBeenUpdated();
    int languageValue();
    void syncSettingsValues(int language, int musicVolume, int effectsVolume, int levelStatuses[], const wchar_t* profileName, size_t length, bool privacyPolicyAccepted); // called from java
    void syncProfileName(const wchar_t* profileName, size_t length); // called from java
    bool privacyPolicyAccepted();
#ifdef __cplusplus
}
#endif



#endif // __APPLICATIONEXPORTS_H__
