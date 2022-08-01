#include "MultiplayerCreateGameMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Screen.h"
#include "Application.h"
#include "Global.h"
#include "AnimatedKnightsForMenu.h"
#include "ObserverTouchEventSubject.h"
#include "Selectionbox.h"
#include "NameGenerator.h"
#include "UDPMessageFactory.h"
#include "Messagebox1.h"
#include "UDP.h"

MultiplayerCreateGameMenu::MultiplayerCreateGameMenu()
: ApplicationMenu(MenuType::MULTIPLAYER_CREATE_GAME)
, animatedKnightsForMenu_(new AnimatedKnightsForMenu()) {

}

void MultiplayerCreateGameMenu::update(double now) noexcept {
    //animatedKnightsForMenu()->init(2);
    if(!Application::widgets().size()) {
        animatedKnightsForMenu()->initSelectionboxForMaps();
        applicationMenu()->initSelectionboxForTime();
    }
    animatedKnightsForMenu_->update(now);
}

bool MultiplayerCreateGameMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_TITLE, info, pixels, 141_ZRD, 65_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_FIELD, info, pixels, 141_ZRD, 108_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_TIME, info, pixels, 141_ZRD, 154_ZRD);
        //drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_TEAM, info, pixels, 141_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_CREATE, info, pixels, 247_ZRD, 235_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_TITLE, info, pixels, 149_ZRD, 65_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_FIELD, info, pixels, 141_ZRD, 108_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_TIME, info, pixels, 141_ZRD, 154_ZRD);
        //drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_TEAM, info, pixels, 141_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_CREATE, info, pixels, 220_ZRD, 235_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_TITLE, info, pixels, 197_ZRD, 65_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_FIELD, info, pixels, 141_ZRD, 108_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_TIME, info, pixels, 141_ZRD, 154_ZRD);
        //drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_TEAM, info, pixels, 141_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_CREATE, info, pixels, 247_ZRD, 235_ZRD);
    }
    drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ARROWS1, info, pixels, 221_ZRD, 111_ZRD);
    drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ARROWS2, info, pixels, 221_ZRD, 157_ZRD);
/*
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    font30px->drawHCentered(&screenL, 71_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__CREATE_GAME));

    auto& font24px = Application::font24px();
    font24px->setColor(0xdd, 0xc6, 0x9f);
    font24px->draw(&screenL, 141_ZRD, 200_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__YOUR_TEAM) + L":");
    font24px->draw(&screenL, 141_ZRD, 111_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__FIELD) + L":");
    font24px->draw(&screenL, 141_ZRD, 158_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__TIME) + L":");
    font24px->drawHCentered(&screenL, 236_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__CREATE));
*/
/*
    auto standingAvatar = static_cast<Application::Avatar>(animatedKnightsForMenu_->standingAvatar());
    if(standingAvatar != Application::avatar()) {
        Application::setAvatar(standingAvatar);
    }
*/
    //animatedKnightsForMenu_->draw(info, pixels);

    if(!offsetsInitialized_) {
        animatedKnightsForMenu_->initOffsets(xOffset_, yOffset_);
        offsetsInitialized_ = true;
    }

    if(offsetsInitialized_ && selectionboxForTime_ && !selectionboxForTimeOffsetUpdated_) {
        selectionboxForTime_->initOffsets(xOffset_, yOffset_);
        selectionboxForTimeOffsetUpdated_ = true;
    }

    return true;
}

bool MultiplayerCreateGameMenu::notify(ObserverTouchEventSubject* subject) noexcept {
    // Let's check the create button first
    bool buttonClicked = Application::language() == TranslateInterface::Language::ENGLISH &&
                         clickTest(subject, 247_ZRD, 323_ZRD, 234_ZRD, 259_ZRD);
    buttonClicked |= Application::language() == TranslateInterface::Language::HUNGARIAN &&
                     clickTest(subject, 220_ZRD, 348_ZRD, 235_ZRD, 259_ZRD);

    buttonClicked |= Application::language() == TranslateInterface::Language::BULGARIAN &&
                     clickTest(subject, 246_ZRD, 322_ZRD, 235_ZRD, 259_ZRD);
    if(buttonClicked) {
        int selectedMapNr = animatedKnightsForMenu_->selectionboxForMaps()->selectedLineNr();
        int selectedMinutesNr = selectionboxForTime_->selectedLineNr();

        Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING);
        int maxPlayersNum{};
        if(selectedMapNr == 0) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP1);
            maxPlayersNum = 2;
        } else if(selectedMapNr == 1) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP2);
            maxPlayersNum = 2;
        } else if(selectedMapNr == 2) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP3);
            maxPlayersNum = 3;
        } else if(selectedMapNr == 3) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP4);
            maxPlayersNum = 3;
        } else if(selectedMapNr == 4) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP5);
            maxPlayersNum = 4;
        } else if(selectedMapNr == 5) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP6);
            maxPlayersNum = 4;
        }

        // Dummy avatar, the SnapshotSyncer will override it
        Application::setAvatar(Application::Avatar::RED);

        NameGenerator ng;
        auto roomName = ng.generate(Application::language(), false);

        UDPMessageInterfaceUPtr msg = UDPMessageFactory::createLobbyRequest(roomName, maxPlayersNum, selectedMapNr, (selectedMinutesNr + 1) * 5);
        UDP udp(loadBalancerHost, loadBalancerPort, true);
        TranslateInterface::Key message = TranslateInterface::Key::TRA__CONNECTING;
        if(!udp.sendMessage2Server(msg)) {
            message = TranslateInterface::Key::TRA__NETWORK_ERROR;
        }

        // Let's pop up a messagebox
        auto messagebox = new Messagebox1(1e6, message);
        Application::showMessageBox(messagebox);
        Application::destroyWidgets();
        Application::setRoomName(roomName);
        //std::wcout << roomName << L"\n";
        return true;
    }
    return false;
}
