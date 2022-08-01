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

/*
namespace {
    TranslateInterface::Language originalLanguage;

    void languageSelectionDrawBeginCallback(Selectionbox* selectionbox) {
        int selectedLine = selectionbox->selectedLineNr();

        originalLanguage = Application::language();
        EnumClassOp eng(TranslateInterface::Key::TRA__ENGLISH);
        EnumClassOp lastOne(TranslateInterface::Key::LAST_ONE);
        for(EnumClassOp i = eng; i < lastOne; ++i) {
            if((i - eng) == selectedLine) {
                Application::setLanguage(Application::translator()->key2Lang(i));
                break;
            }
        }
    }

    void languageSelectionDrawEndCallback([[maybe_unused]]Selectionbox* selectionbox) {
        Application::setLanguage(originalLanguage);
    }
}
*/

SettingsMenu::SettingsMenu()
: ApplicationMenu(MenuType::SETTINGS) {

}

void SettingsMenu::update([[maybe_unused]]double now) noexcept {
    if(!Application::widgets().size()) {
        initSelectionboxForLanguages();
        initSlidersForAudioControl();
    }
}


bool SettingsMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}
    drawRareMatrix(NATIVE_BITMAP_PROFILE_RED, info, pixels, + xOffset_ + 387_ZRD, + yOffset_ + 72_ZRD);

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        auto saveData = bitmapData(NATIVE_BITMAP_SETTINGS_MENU_ENG_SAVE, false);
        saveButtonRect_ = Rect(259_ZRD, 229_ZRD, saveData->width(), saveData->height());
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_ENG_TITLE, info, pixels, 221_ZRD, 71_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_ENG_LANGUAGE, info, pixels, 124_ZRD, 116_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_ENG_MUSIC, info, pixels, 125_ZRD, 158_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_ENG_EFFECTS, info, pixels, 124_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_ENG_SAVE, info, pixels, saveButtonRect_.x, saveButtonRect_.y);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        auto saveData = bitmapData(NATIVE_BITMAP_SETTINGS_MENU_HUN_SAVE, false);
        saveButtonRect_ = Rect(246_ZRD, 229_ZRD, saveData->width(), saveData->height());
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_HUN_TITLE, info, pixels, 197_ZRD, 71_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_HUN_LANGUAGE, info, pixels, 124_ZRD, 116_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_HUN_MUSIC, info, pixels, 124_ZRD, 158_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_HUN_EFFECTS, info, pixels, 124_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_HUN_SAVE, info, pixels, saveButtonRect_.x, saveButtonRect_.y);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        auto saveData = bitmapData(NATIVE_BITMAP_SETTINGS_MENU_BUL_SAVE, false);
        saveButtonRect_ = Rect(246_ZRD, 229_ZRD, saveData->width(), saveData->height());
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_BUL_TITLE, info, pixels, 213_ZRD, 71_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_BUL_LANGUAGE, info, pixels, 124_ZRD, 116_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_BUL_MUSIC, info, pixels, 125_ZRD, 158_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_BUL_EFFECTS, info, pixels, 124_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_BUL_SAVE, info, pixels, saveButtonRect_.x, saveButtonRect_.y);
    }
    drawMenuItem(NATIVE_BITMAP_SETTINGS_MENU_ARROWS, info, pixels, 256_ZRD, 119_ZRD);
/*
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    font30px->drawHCentered(&screenL, 80_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__SETTINGS));

    auto& font24px = Application::font24px();
    font24px->setColor(0xdd, 0xc6, 0x9f);
    font24px->draw(&screenL, 126_ZRD, 141_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__LANGUAGE) + L":");
    font24px->drawHCentered(&screenL, 232_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__SAVE));
*/
    if(!offsetsInitialized_) {
        selectionBox_->setX(selectionBox_->x() + xOffset_);
        selectionBox_->setY(selectionBox_->y() + yOffset_);
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
    selectionBox_->setY(125_ZRD);
    selectionBox_->setWidth(418_ZRD - 268_ZRD);
    selectionBox_->setHeight(20_ZRD);
    //selectionBox_->addDrawBeginCallback(std::bind(languageSelectionDrawBeginCallback, std::placeholders::_1));
    //selectionBox_->addDrawEndCallback(std::bind(languageSelectionDrawEndCallback, std::placeholders::_1));
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

void SettingsMenu::initSlidersForAudioControl() noexcept {
    offsetsInitialized_ = false;
    musicSlider_ = new Slider(0, 0, 0, 100);
    musicSlider_->setX(268_ZRD);
    musicSlider_->setY(158_ZRD);
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
    effectSlider_->setY(200_ZRD);
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
        Application::setLanguage(lang);
        Application::saveSettings();
        Application::goUpInTheMenu();
        return true;
    }
    return false;
}

void SettingsMenu::musicVolumeValueChangedCallback([[maybe_unused]]Slider* slider, int value) noexcept {
    //debug("musicVolumeValue_: %d", musicVolumeValue_);
    setMusicVolume(value);
}

void SettingsMenu::effectVolumeValueChangedCallback([[maybe_unused]]Slider* slider, int value) noexcept {
    //debug("effectVolumeValue_: %d", effectVolumeValue_);
    setEffectVolume(value);
    SoundEffect::play(SoundEffect::Name::STOP_PLAYING_GAME_BALL_TAP);
    SoundEffect::play(SoundEffect::Name::PLAY_GAME_BALL_TAP);
}

void SettingsMenu::resetVolumes() noexcept {
    setMusicVolume(musicVolumeValue_);
    setEffectVolume(effectVolumeValue_);
}
