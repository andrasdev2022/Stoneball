#include "MultiplayerFindGameMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Screen.h"
#include "Application.h"
#include "Global.h"
#include "AnimatedKnightsForMenu.h"
#include "Listbox1.h"
#include "Knight.h"
#include "UDPMessageFactory.h"
#include "Messagebox1.h"
#include "ObserverTouchEventSubject.h"
#include "UDP.h"

MultiplayerFindGameMenu::MultiplayerFindGameMenu()
: ApplicationMenu(MenuType::MULTIPLAYER_FIND_GAME)
, animatedKnightsForMenu_(new AnimatedKnightsForMenu()) {

}

void MultiplayerFindGameMenu::update(double now) noexcept {
    animatedKnightsForMenu_->update(now);
}


bool MultiplayerFindGameMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_FIND_GAME_MENU_BACKGROUND, false), info, pixels)) {return false;}

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_ENG_TITLE, info, pixels, 157_ZRD, 65_ZRD);
        //drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_ENG_TEAM, info, pixels, 145_ZRD, 208_ZRD);
        drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_ENG_JOIN, info, pixels, 259_ZRD, 235_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_HUN_TITLE, info, pixels, 173_ZRD, 65_ZRD);
        //drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_HUN_TEAM, info, pixels, 145_ZRD, 208_ZRD);
        drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_HUN_JOIN, info, pixels, 213_ZRD, 235_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_BUL_TITLE, info, pixels, 197_ZRD, 65_ZRD);
        //drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_BUL_TEAM, info, pixels, 145_ZRD, 208_ZRD);
        drawMenuItem(NATIVE_BITMAP_FIND_GAME_MENU_BUL_JOIN, info, pixels, 200_ZRD, 235_ZRD);
    }
/*
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    font30px->drawHCentered(&screenL, 71_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__FIND_GAME));

    auto& font24px = Application::font24px();
    font24px->setColor(0xdd, 0xc6, 0x9f);
    font24px->draw(&screenL, 145_ZRD, 208_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__TEAM) + L":");
    font24px->drawHCentered(&screenL, 237_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__JOIN));
*/
    animatedKnightsForMenu_->draw(info, pixels);
    if(!listboxOffsetUpdated_) {
        for(auto& widget : Application::widgets()) {
            if(widget->id() == listbox1Id_) {
                listboxOffsetUpdated_ = true;
                widget->setX(widget->x() + xOffset_);
                widget->setY(widget->y() + yOffset_);
                break;
            }
        }
    }

    return true;
}

// We have to change the avatar types/images here
void MultiplayerFindGameMenu::listboxSelectionCallback([[maybe_unused]]Listbox1* listbox, [[maybe_unused]]int selectedLineNr) noexcept {
    //debug("listbox: %p, selected line number: %d", listbox, selectedLineNr);
    animatedKnightsForMenu_->clear();
}

namespace Application {
    extern THREAD_LOCAL std::string serverHost_;
    extern THREAD_LOCAL int serverPort_;
    extern THREAD_LOCAL bool createConnection_;
    extern THREAD_LOCAL double lastMessageArrivalTime_;
}

bool MultiplayerFindGameMenu::notify(ObserverTouchEventSubject* subject) noexcept {
    bool buttonClicked = Application::language() == TranslateInterface::Language::ENGLISH &&
                         clickTest(subject, 259_ZRD, 309_ZRD, 235_ZRD, 259_ZRD);
    buttonClicked |= Application::language() == TranslateInterface::Language::HUNGARIAN &&
                     clickTest(subject, 213_ZRD, 354_ZRD, 235_ZRD, 259_ZRD);

    buttonClicked |= Application::language() == TranslateInterface::Language::BULGARIAN &&
                     clickTest(subject, 200_ZRD, 367_ZRD, 235_ZRD, 259_ZRD);
    if(buttonClicked) {
        // Join button
        // Is anything selected from the list?
        auto& widgets = Application::widgets();
        if(!widgets.size()) return false;
        auto listboxIt = std::find_if(widgets.begin(), widgets.end(), [](const BlueTriangleUPtr& widget){return widget->listbox1();});
        if(listboxIt == widgets.end()) return false;

        if((*listboxIt)->listbox1()->selectedLineNr() == -1) return false;

        auto selectedItem = (*listboxIt)->listbox1()->selectedItem();
        //debug("selected item: %s, data: %s", selectedItem.label.c_str(), selectedItem.itemData.c_str());
        //std::wcout << L"label: " << selectedItem.label << ", itemdata: " << selectedItem.itemData << std::endl;

        size_t pos = selectedItem.itemData.find(L' ');
        auto host = selectedItem.itemData.substr(0, pos);
        size_t pos2 = selectedItem.itemData.find(L' ', pos + 1);
        auto portStr = selectedItem.itemData.substr(pos + 1, pos2 - pos);
        int port = std::stoi(portStr.c_str());

        pos = pos2;
        pos2 = selectedItem.itemData.find(L' ', pos + 1);
        auto mapIndexStr = selectedItem.itemData.substr(pos + 1, pos2 - pos - 1);
        int mapIndex = std::stoi(mapIndexStr.c_str());

        pos = pos2;
        //pos2 = selectedItem.itemData.find(' ', pos + 1);
        auto roomName = selectedItem.itemData.substr(pos + 1);

        // Dummy avatar, the snapshotsyncer will set it
        setAvatar(Application::Avatar::RED);

        Application::serverHost_ = std::string(host.begin(), host.end());
        Application::serverPort_ = port;
        Application::createConnection_ = false;
        Application::lastMessageArrivalTime_ = now_ms();
        UDP udp(Application::serverHost_, Application::serverPort_, true);
        UDPMessageInterfaceUPtr msg = UDPMessageFactory::createJoinRequest();
        TranslateInterface::Key message = TranslateInterface::Key::TRA__CONNECTING;
        if(!udp.sendMessage2Server(msg)) {
            message = TranslateInterface::Key::TRA__NETWORK_ERROR;
        }

        // Let's pop up a messagebox
        auto messagebox = new Messagebox1(1e6, message);
        Application::showMessageBox(messagebox);
        Application::destroyWidgets();
        Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING);
        if(mapIndex == 0) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP1);
        } else if(mapIndex == 1) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP2);
        } else if(mapIndex == 2) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP3);
        } else if(mapIndex == 3) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP4);
        } else if(mapIndex == 4) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP5);
        } else if(mapIndex == 5) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP6);
        }

        Application::setRoomName(roomName);
        //std::wcout << roomName << L"\n";
        return true;
    }/* else {
        return avatarSelectionCheckSimple(subject);
    }*/
    return false;
}

Listbox1* MultiplayerFindGameMenu::initListbox() noexcept {
    listboxOffsetUpdated_ = false;
    auto listbox = new Listbox1(0, 0, 82_ZRD);
    auto callback = std::bind(&MultiplayerFindGameMenu::listboxSelectionCallback, this,
                              std::placeholders::_1, std::placeholders::_2);
    listbox->add(callback);
    listbox->setX(159_ZRD);
    listbox->setY(105_ZRD);
    listbox1Id_ = listbox->id();
    return listbox;
}
