#include "ApplicationExports.h"
#include "Application.h"
#include "ClientTimeSyncer.h"
#include "TouchEventHandler.h"
#include "Algorithms.h"

using namespace Algorithms;

namespace Application {

extern THREAD_LOCAL ClientTimeSyncer syncer_;
extern THREAD_LOCAL double now__;
extern THREAD_LOCAL bool settingMustBeSaved_;
extern THREAD_LOCAL TranslateInterface::Language language_;
extern THREAD_LOCAL MenuInterfaceSPtr activeMenuI_;

extern int musicVolume() noexcept;
extern int effectVolume() noexcept;
extern void setMusicVolume(int volume) noexcept;
extern void setEffectVolume(int volume) noexcept;

} // namespace Application

void drawWorld(AndroidBitmapInfo* info, void* pixels) {
    Application::drawWorld(info, pixels);
}

void updateWorld(double now) {
#ifndef __SERVERAPP__
    // syncer must be called for multiplayer clients
    if(Application::activeMenuI_ && anyOf(Application::activeMenu(), MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING, MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING)) {
        now = Application::syncer_.now();
    }
#endif
    Application::updateWorld(now);
}

void onTouchEvent(int action, double x, double y, double now) {
    // syncer must be called for multiplayer clients
    if(Application::activeMenuI_ && anyOf(Application::activeMenu(), MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING, MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING)) {
        now = Application::syncer_.now();
    }
    Application::now__ = now;
    Application::touchEventHandler()->onTouchEvent(action, x, y);
    //debug("onTouchEvent x: %f, y: %f", x, y);
}

void wakeUp() {
    Application::wakeUp();
}

int musicVolume() {return Application::musicVolume();}

int effectVolume() {return Application::effectVolume();}

void setMusicVolume(int volume) {Application::setMusicVolume(volume);}

void setEffectVolume(int volume) {Application::setEffectVolume(volume);}

int* levelStatuses() {
    return Application::levelStatuses();
}

bool hasSettingsBeenUpdated() {
    if(Application::settingMustBeSaved_) {
        Application::settingMustBeSaved_ = false;
        return true;
    }
    return false;
}

int languageValue() {
    return (int)Application::language_;
}

void syncSettingsValues(int language, int musicVolume, int effectsVolume, int levelStatus[], const wchar_t* profileName, size_t length,
bool privacyPolicyAccepted) {
    Application::setLanguage(static_cast<TranslateInterface::Language>(language));
    Application::setMusicVolume(musicVolume);
    Application::setEffectVolume(effectsVolume);
    Application::setLevelStatuses(levelStatus);
    Application::setProfileName(std::wstring(profileName + 1, profileName + length));
    Application::privacyPolicyAccepted() = privacyPolicyAccepted;
}

void syncProfileName(const wchar_t* profileName, size_t length) {
    // First byte is BOM (0xfeff), so we skip it, hence the "profileName + 1".
    // There IS NO null-terminator.
    Application::setProfileName(std::wstring(profileName + 1, profileName + length));
}

bool privacyPolicyAccepted() {
    return Application::privacyPolicyAccepted();
}