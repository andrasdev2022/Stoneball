#include "SettingsMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Selectionbox.h"
#include "Application.h"
#include "EnumClassOp.h"
#include "Global.h"
#include "TouchEventHandler.h"
#include "ObserverTouchEventSubject.h"
#include "Slider.h"
#include "SoundEffect.h"
#include "DesktopBitmapIds.h"

SettingsMenu::SettingsMenu()
: ApplicationMenu(MenuType::SETTINGS) {

}

void SettingsMenu::update([[maybe_unused]]double now) noexcept {
    if(!Application::widgets().size()) {
        initSelectionboxForLanguages();
        initSelectionboxForResolution();
        initSlidersForAudioControl();
    }
}


bool SettingsMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}
    drawRareMatrix(NATIVE_BITMAP_PROFILE_RED, info, pixels, + xOffset_ + 387_ZRD, + yOffset_ + 72_ZRD);

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        auto saveData = bitmapData(DesktopBitmapIds::settings_menu_eng_save(), false);
        saveButtonRect_ = Rect(260_ZRD, 243_ZRD, saveData->width(), saveData->height());
        drawMenuItem(DesktopBitmapIds::settings_menu_eng_title(), info, pixels, 221_ZRD, 75_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_eng_language(), info, pixels, 125_ZRD, 118_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_eng_music(), info, pixels, 125_ZRD, 144_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_eng_effects(), info, pixels, 125_ZRD, 175_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_eng_resolution(), info, pixels, 125_ZRD, 205_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_eng_save(), info, pixels, saveButtonRect_.x, saveButtonRect_.y);
        drawMenuItem(DesktopBitmapIds::settings_menu_eng_arrows2(), info, pixels, 274_ZRD, 202_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        auto saveData = bitmapData(DesktopBitmapIds::settings_menu_hun_save(), false);
        saveButtonRect_ = Rect(246_ZRD, 241_ZRD, saveData->width(), saveData->height());
        drawMenuItem(DesktopBitmapIds::settings_menu_hun_title(), info, pixels, 198_ZRD, 75_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_hun_language(), info, pixels, 125_ZRD, 110_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_hun_music(), info, pixels, 125_ZRD, 140_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_hun_effects(), info, pixels, 125_ZRD, 171_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_hun_resolution(), info, pixels, 125_ZRD, 203_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_hun_save(), info, pixels, saveButtonRect_.x, saveButtonRect_.y);
        drawMenuItem(DesktopBitmapIds::settings_menu_hun_arrows2(), info, pixels, 256_ZRD, 200_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        auto saveData = bitmapData(DesktopBitmapIds::settings_menu_bul_save(), false);
        saveButtonRect_ = Rect(246_ZRD, 245_ZRD, saveData->width(), saveData->height());
        drawMenuItem(DesktopBitmapIds::settings_menu_bul_title(), info, pixels, 214_ZRD, 76_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_bul_language(), info, pixels, 125_ZRD, 111_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_bul_music(), info, pixels, 125_ZRD, 144_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_bul_effects(), info, pixels, 125_ZRD, 176_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_bul_resolution(), info, pixels, 125_ZRD, 209_ZRD);
        drawMenuItem(DesktopBitmapIds::settings_menu_bul_save(), info, pixels, saveButtonRect_.x, saveButtonRect_.y);
        drawMenuItem(DesktopBitmapIds::settings_menu_bul_arrows2(), info, pixels, 256_ZRD, 204_ZRD);
    }
    drawMenuItem(DesktopBitmapIds::settings_menu_arrows(), info, pixels, 256_ZRD, 110_ZRD);

    if(!offsetsInitialized_) {
        selectionBox_->setX(selectionBox_->x() + xOffset_);
        selectionBox_->setY(selectionBox_->y() + yOffset_);
        resolutionSelectionBox_->setX(resolutionSelectionBox_->x() + xOffset_);
        resolutionSelectionBox_->setY(resolutionSelectionBox_->y() + yOffset_);
        musicSlider_->setX(musicSlider_->x() + xOffset_);
        musicSlider_->setY(musicSlider_->y() + yOffset_);
        effectSlider_->setX(effectSlider_->x() + xOffset_);
        effectSlider_->setY(effectSlider_->y() + yOffset_);
        offsetsInitialized_ = true;
    }
    return true;
}


void SettingsMenu::initSelectionboxForLanguages() noexcept {
    offsetsInitialized_ = false;
    selectionBox_ = new Selectionbox(0, 0);
    selectionBox_->setX(268_ZRD);
    selectionBox_->setY(110_ZRD);
    selectionBox_->setWidth(418_ZRD - 268_ZRD);
    selectionBox_->setHeight(20_ZRD);
    std::wstring label;

    auto language = Application::language();
    EnumClassOp eng(TranslateInterface::Key::TRA__ENGLISH);
    EnumClassOp lastOne(TranslateInterface::Key::LAST_ONE);
    EnumClassOp selected = eng;
    for(EnumClassOp i = eng; i < lastOne; ++i) {
        if(i == Application::translator()->lang2Key(language)) {selected = i;}
        Application::setLanguage(Application::translator()->key2Lang(i));
        label = (*Application::translator())((TranslateInterface::Key)i);
        selectionBox_->addItem(Selectionbox::Item{label, std::to_wstring((int)i)});
    }
    Application::setLanguage(language);
    selectionBox_->setSelectedLineNr(((int)selected - (int)eng));
    selectionBox_->setName("LANGUAGES");
    Application::touchEventHandler()->add(selectionBox_);
    Application::showWidget(selectionBox_);
}

void SettingsMenu::initSelectionboxForResolution() noexcept {
    offsetsInitialized_ = false;
    resolutionSelectionBox_ = new Selectionbox(0, 0);

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        resolutionSelectionBox_->setX(286_ZRD);
        resolutionSelectionBox_->setY(202_ZRD);
        resolutionSelectionBox_->setWidth(428_ZRD - 287_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        resolutionSelectionBox_->setX(268_ZRD);
        resolutionSelectionBox_->setY(200_ZRD);
        resolutionSelectionBox_->setWidth(428_ZRD - 268_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        resolutionSelectionBox_->setX(268_ZRD);
        resolutionSelectionBox_->setY(204_ZRD);
        resolutionSelectionBox_->setWidth(428_ZRD - 268_ZRD);
    }

    resolutionSelectionBox_->setHeight(20_ZRD);

    resolutionSelectionBox_->addItem(Selectionbox::Item{L"569x320", std::to_wstring(0)});
    resolutionSelectionBox_->addItem(Selectionbox::Item{L"853x480", std::to_wstring(1)});
    resolutionSelectionBox_->addItem(Selectionbox::Item{L"1138x640", std::to_wstring(2)});
    resolutionSelectionBox_->addItem(Selectionbox::Item{L"1422x800", std::to_wstring(3)});
    resolutionSelectionBox_->addItem(Selectionbox::Item{L"1707x960", std::to_wstring(4)});

    resolutionSelectionBox_->setSelectedLineNr(Application::resolution());
    resolutionSelectionBox_->setName("RESOLUTION");

    Application::touchEventHandler()->add(resolutionSelectionBox_);
    Application::showWidget(resolutionSelectionBox_);
}

void SettingsMenu::initSlidersForAudioControl() noexcept {
    offsetsInitialized_ = false;
    musicSlider_ = new Slider(0, 0, 0, 100);
    musicSlider_->setX(268_ZRD);
    musicSlider_->setY(144_ZRD);
    musicSlider_->setWidth(418_ZRD - 268_ZRD);
    musicSlider_->setHeight(20_ZRD);
    musicSlider_->setName("MUSICCONTROL");
    musicSlider_->setValue(musicVolume());
    musicVolumeValue_ = musicVolume();
    musicSlider_->add(std::bind(&SettingsMenu::musicVolumeValueChangedCallback, this, std::placeholders::_1, std::placeholders::_2));
    Application::touchEventHandler()->add(musicSlider_);
    Application::showWidget(musicSlider_);

    effectSlider_ = new Slider(0, 0, 0, 100);
    effectSlider_->setX(268_ZRD);
    effectSlider_->setY(176_ZRD);
    effectSlider_->setWidth(418_ZRD - 268_ZRD);
    effectSlider_->setHeight(20_ZRD);
    effectSlider_->setName("EFFECTCONTROL");
    effectSlider_->setValue(effectVolume());
    effectVolumeValue_ = effectVolume();
    effectSlider_->add(std::bind(&SettingsMenu::effectVolumeValueChangedCallback, this, std::placeholders::_1, std::placeholders::_2));
    Application::touchEventHandler()->add(effectSlider_);
    Application::showWidget(effectSlider_);
}

bool SettingsMenu::notify(ObserverTouchEventSubject* subject) noexcept {
    bool buttonPressed = false;

    if(clickTest(subject, saveButtonRect_.x, saveButtonRect_.x + saveButtonRect_.width, saveButtonRect_.y, saveButtonRect_.y + saveButtonRect_.height)) {
        buttonPressed = true;
    }

    if(!buttonPressed && clickTest(subject, 387_ZRD, 420_ZRD, 72_ZRD, 101_ZRD)) {
        // open the profile dialog implemented in java
        extern char callProfileActivity;
        callProfileActivity = 'y';
    }

    if(buttonPressed) {
        auto item = selectionBox_->selectedItem();
        auto key = static_cast<TranslateInterface::Key>(std::stoi(item.itemData));
        auto lang = Application::translator()->key2Lang(key);
        musicVolumeValue_ = musicVolume();
        effectVolumeValue_ = effectVolume();
        resolutionIndex_ = resolutionSelectionBox_->selectedLineNr();
        Application::setLanguage(lang);
        Application::setResolution(resolutionIndex_);
        Application::saveSettings();
        Application::goUpInTheMenu();
        return true;
    }
    return false;
}

void SettingsMenu::musicVolumeValueChangedCallback([[maybe_unused]]Slider* slider, int value) noexcept {
    setMusicVolume(value);
}

void SettingsMenu::effectVolumeValueChangedCallback([[maybe_unused]]Slider* slider, int value) noexcept {
    setEffectVolume(value);
    SoundEffect::play(SoundEffect::Name::STOP_PLAYING_GAME_BALL_TAP);
    SoundEffect::play(SoundEffect::Name::PLAY_GAME_BALL_TAP);
}

void SettingsMenu::resetVolumes() noexcept {
    setMusicVolume(musicVolumeValue_);
    setEffectVolume(effectVolumeValue_);
}
