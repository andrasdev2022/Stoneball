#ifndef __TRANSLATEINTERFACE_H__
#define __TRANSLATEINTERFACE_H__

#include <string>
#include <memory>

class TranslateInterface {
public:
    enum class Language {
        ENGLISH,
        BULGARIAN,
        HUNGARIAN,
        //GERMAN,
        //FRENCH,
        //RUSSIAN,

        LAST_ONE
    };

    enum class Key {
        TRA__TEAM,
        TRA__RED_TEAM,
        TRA__DARK_TEAM,
        TRA__BLONDE_TEAM,
        TRA__GREEN_TEAM,
        TRA__MY_TEAM,
        TRA__YOUR_TEAM,
        TRA__RED_TURN,
        TRA__DARK_TURN,
        TRA__BLONDE_TURN,
        TRA__GREEN_TURN,
        TRA__CONNECTING,
        TRA__MULTIPLAYER_MAP1_LABEL,
        TRA__MULTIPLAYER_MAP2_LABEL,
        TRA__MULTIPLAYER_MAP3_LABEL,
        TRA__MULTIPLAYER_MAP4_LABEL,
        TRA__MULTIPLAYER_MAP5_LABEL,
        TRA__MULTIPLAYER_MAP6_LABEL,
        TRA__MINUTES,
        TRA__YOU_LOST,
        TRA__YOU_WON,
        TRA__LABEL_PASS,
        TRA__LABEL_NEXT_TURN,
        TRA__ROOM_NAME,
        TRA__GAME_HAS_BEGUN,
        TRA__GRAY_WON,
        TRA__RED_WON,
        TRA__BLONDE_WON,
        TRA__GREEN_WON,
        TRA__CONNECTION_REJECTED,
        TRA__SELECT_AN_AVATAR,
        TRA__FIELD,
        TRA__LENGTH,
        TRA__TIME,
        TRA__START,
        TRA__CREATE,
        TRA__JOIN,
        TRA__DURATION,
        TRA__LEVELS,
        TRA__MULTIPLAYER,
        TRA__SETTINGS,
        TRA__ABOUT,
        TRA__ON_ONE_DEVICE,
        TRA__CREATE_GAME,
        TRA__FIND_GAME,
        TRA__SAVE,
        TRA__LANGUAGE,
        TRA__WAITING_FOR_OPPONENT,
        TRA__NO_OPPONNENTS_SHOWED_UP,
        TRA__NETWORK_ERROR,
        TRA__VERSION_DISCREPANCY,
        TRA__SOMEONES_TURN,
        TRA__PUSHES,
        TRA__LEVEL,

        TRA__ENGLISH,
        TRA__BULGARIAN,
        TRA__HUNGARIAN,
        //TRA__GERMAN,
        //TRA__FRENCH,
        //TRA__RUSSIAN,
        LAST_ONE
    };

    virtual ~TranslateInterface() = default;
    virtual std::wstring operator()(Key key) const = 0;
    virtual std::wstring operator()(Language key) const = 0;
    virtual std::wstring str(TranslateInterface::Key key) const noexcept = 0;
    virtual Language language() const noexcept = 0;

    virtual Key lang2Key(Language lng) const noexcept {
        switch(lng) {
        case Language::LAST_ONE:
        case Language::ENGLISH:   return Key::TRA__ENGLISH;
        case Language::BULGARIAN: return Key::TRA__BULGARIAN;
        case Language::HUNGARIAN: return Key::TRA__HUNGARIAN;
        //case Language::GERMAN:    return Key::TRA__GERMAN;
        //case Language::FRENCH:    return Key::TRA__FRENCH;
        //case Language::RUSSIAN:   return Key::TRA__RUSSIAN;
        }
        return Key::TRA__ENGLISH;
    }
    virtual Language key2Lang(Key key) const noexcept {
        switch(key) {
        case Key::TRA__BULGARIAN: return Language::BULGARIAN; break;
        case Key::TRA__HUNGARIAN: return Language::HUNGARIAN; break;
        //case Key::TRA__GERMAN:    return Language::GERMAN; break;
        //case Key::TRA__FRENCH:    return Language::FRENCH; break;
        //case Key::TRA__RUSSIAN:   return Language::RUSSIAN; break;
        case Key::TRA__ENGLISH:
        default: return Language::ENGLISH; break;
        }
        return Language::ENGLISH;
    }
};

using TranslateInterfaceUPtr = std::unique_ptr<TranslateInterface>;
#endif // __TRANSLATEINTERFACE_H__
