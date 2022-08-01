#include "Application.h"
#include "Defines.h"
#include "UDPMessageFactory.h"
#include "UDP.h"
#include "UDPServerInterface.h"
#include "UDPServerFactory.h"
#include "UDPMessageQueueFactory.h"
#include "CommonUtil.h"
#include "ClientTimeSyncer.h"
#include "UDPServerSnapshot.h"
#include "UDPServerDeltaSnapshot.h"
#include "MultiplayerWorld.h"
#include "UDPLobbyData.h"
#include "TouchEventHandler.h"
#include "UDPRoomListAnswer.h"
#include "Listbox1.h"
#include "MediaData.h"
#include "BitmapIds.h"
#include "Messagebox1.h"
#include "MultiplayerFindGameMenu.h"
#include "Tileset.h"
#include "BuildNumber.h"

namespace Application {

THREAD_LOCAL UDPServerInterfaceUPtr udpServer_;
THREAD_LOCAL UDPMessageQueueInterfaceUPtr udpMessageQueue_;
THREAD_LOCAL ClientTimeSyncer syncer_;
THREAD_LOCAL double lastMessageArrivalTime_{};
extern THREAD_LOCAL std::vector<BlueTriangleUPtr> messageboxes_;

[[nodiscard]] UDPServerInterface* udpServer() noexcept {
    return udpServer_.get();
}

void processServerSnapshot(UDPMessageInterfaceUPtr& msg) noexcept {
    // we have a connection, let's remove the client message boxes
    if(!tileset() || !tileset()->gameOver()) {
        destroyMessageBoxes();
    }
    //destroyWidgets(); // Why is this here?

    syncer_.onServerUpdate(msg->serverSnapshot()->timeStamp());
    syncer_.trace();
    // it must be applied on world
    if(playerMode() == PlayerMode::MULTIPLAYER && avatar() != Avatar::UNDEFINED && game() && game()->worldList().size()) {
        WorldUPtr world = std::unique_ptr<World>(latestWorld()->copy());
        Application::game()->updateWorldOfButtons(world);
        auto retv = game()->worldList().emplace(msg->serverSnapshot()->timeStamp(), std::move(world));
        if(!retv.second) {
            debug("ERROR: worldList().emplace failure 1, %f", msg->serverSnapshot()->timeStamp());
        } else if(latestWorld()->multiplayer()) { // do not sync in case of packet duplication
            latestWorld()->multiplayer()->sync(msg->serverSnapshot());
        }
    }
}

void processServerDeltaSnapshot(UDPMessageInterfaceUPtr& msg) noexcept {
    // TODO: check wether this is correct here
    if(!tileset() || !tileset()->gameOver()) {
        destroyMessageBoxes();
    }

    syncer_.onServerUpdate(msg->serverDeltaSnapshot()->timeStamp());
    syncer_.trace();
    // it must be applied on world
    if(playerMode() == PlayerMode::MULTIPLAYER && avatar() != Avatar::UNDEFINED && game() && game()->worldList().size()) {
        WorldUPtr world = std::unique_ptr<World>(latestWorld()->copy());
        Application::game()->updateWorldOfButtons(world);
        auto retv = game()->worldList().emplace(msg->serverDeltaSnapshot()->timeStamp(), std::move(world));
        if(!retv.second) {
            debug("ERROR: worldList().emplace failure 2 (timestamp: %lu)", msg->serverDeltaSnapshot()->timeStamp());
        } else if(latestWorld()->multiplayer()) { // do not sync in case of packet duplication
            latestWorld()->multiplayer()->sync(msg->serverDeltaSnapshot());
        }
    }
}

void processRoomListAnswer(UDPMessageInterfaceUPtr& msg) noexcept {
    destroyMessageBoxes();
    if(playerMode() == PlayerMode::MULTIPLAYER && activeMenu() == MenuInterface::MenuType::MULTIPLAYER_FIND_GAME) {
        auto menuItem = Application::activeMenuItem()->multiplayerFindGameMenu();
        auto listbox = menuItem->initListbox();
        auto roomListAnswer = msg->roomListAnswer();
        auto& tr = Application::translator();
        for(auto& item : roomListAnswer->roomList()) {
            std::wstring viewOnly;
            std::wstring label = item.roomName;
            std::wstring userData = std::wstring(item.host.begin(), item.host.end()) + L' ' + std::to_wstring(item.port) + L' ';
            if(item.selectedMapNr == 0) {
                userData += L"0";
                label += L", " + (*tr)(TranslateInterface::Key::TRA__MULTIPLAYER_MAP1_LABEL);
                if(item.clientNum > 1) viewOnly = L" (v)";
            } else if(item.selectedMapNr == 1) {
                userData += L"1";
                label += L", " + (*tr)(TranslateInterface::Key::TRA__MULTIPLAYER_MAP2_LABEL);
                if(item.clientNum > 1) viewOnly = L" (v)";
            } else if(item.selectedMapNr == 2) {
                userData += L"2";
                label += L", " + (*tr)(TranslateInterface::Key::TRA__MULTIPLAYER_MAP3_LABEL);
                if(item.clientNum > 2) viewOnly = L" (v)";
            } else if(item.selectedMapNr == 3) {
                userData += L"3";
                label += L", " + (*tr)(TranslateInterface::Key::TRA__MULTIPLAYER_MAP4_LABEL);
                if(item.clientNum > 2) viewOnly = L" (v)";
            } else if(item.selectedMapNr == 4) {
                userData += L"4";
                label += L", " + (*tr)(TranslateInterface::Key::TRA__MULTIPLAYER_MAP5_LABEL);
                if(item.clientNum > 3) viewOnly = L" (v)";
            } else if(item.selectedMapNr == 5) {
                userData += L"5";
                label += L", " + (*tr)(TranslateInterface::Key::TRA__MULTIPLAYER_MAP6_LABEL);
                if(item.clientNum > 3) viewOnly = L" (v)";
            }
            label += L", " + std::to_wstring(item.selectedMinutesNr) + L" " + (*tr)(TranslateInterface::Key::TRA__MINUTES);
            userData += L" " + item.roomName;

            label += viewOnly;
            listbox->addItem(Listbox1::Item{label, userData});
        }
        touchEventHandler()->add(listbox);
        showWidget(listbox);
    }
}

THREAD_LOCAL std::string serverHost_;
THREAD_LOCAL int serverPort_{};
THREAD_LOCAL bool createConnection_{};

void processLobbyData(UDPMessageInterfaceUPtr& msg) noexcept {
    debug("DBG Lobby ptr: %p", (void*)msg->lobbyData());
    debug("DBG Lobby host: %s", msg->lobbyData()->host().c_str());
    debug("DBG Lobby port: %u", msg->lobbyData()->port());
    debug("DBG dbg avatar: %d", (int)avatar());
    createConnection_ = true;
    UDPMessageInterfaceUPtr newMsg = UDPMessageFactory::createConnectionRequest();
    serverHost_ = msg->lobbyData()->host();
    serverPort_ = msg->lobbyData()->port();
    UDP udp(serverHost_, serverPort_, true);
    [[maybe_unused]]bool xx = udp.sendMessage2Server(newMsg);
}

void processLobbyRejected(UDPMessageInterfaceUPtr& msg) noexcept {
    destroyMessageBoxes();

    TranslateInterface::Key message = TranslateInterface::Key::TRA__CONNECTION_REJECTED;
    if(msg->buildNumber() != BuildNumber::n()) {
        debug("build number discrepancy: %u %u", msg->buildNumber(), BuildNumber::n());
        message = TranslateInterface::Key::TRA__VERSION_DISCREPANCY;
    }
    auto messagebox = new Messagebox1(1e30, message);
    Application::showMessageBox(messagebox);
}

void clearServerMessages() noexcept {
    if(!udpServer_) return;
    udpServer_->readClientConnection(UDP::clientConnection());
    while(udpMessageQueue_->size()) {
        udpMessageQueue_->pop();
    }
}

// Only Clients use it in multiplayer mode
void processServerMessages(bool findGame) noexcept {
    if(!udpServer_) {
        udpMessageQueue_ = UDPMessageQueueFactory::create();
        udpServer_ = UDPServerFactory::create(udpMessageQueue_.get());
         // The client does NOT need a server thread, it reads the connection once it was made
    }

    udpServer_->readClientConnection(UDP::clientConnection());
    while(udpMessageQueue_->size()) {
        UDPMessageInterfaceUPtr msg = udpMessageQueue_->pop();
        if(!msg) {
            // TODO: log an error or something
            debug("ERROR: processServerMessages: nullptr from UDP message queue");
            continue;
        }

        lastMessageArrivalTime_ = now_ms();
        switch(msg->typeId()) {
        case UDPMessageInterface::TypeId::SERVER_SNAPSHOT: processServerSnapshot(msg); break;
        case UDPMessageInterface::TypeId::SERVER_DELTA_SNAPSHOT: processServerDeltaSnapshot(msg); break;
        case UDPMessageInterface::TypeId::LOBBY_DATA: processLobbyData(msg); break;
        case UDPMessageInterface::TypeId::ROOM_LIST_ANSWER: processRoomListAnswer(msg); break;
        case UDPMessageInterface::TypeId::LOBBY_REJECTED: processLobbyRejected(msg); break;
        // --------------------------------------------------
        case UDPMessageInterface::TypeId::CONNECTION_REQUEST: /*server won't send it*/
        case UDPMessageInterface::TypeId::RECONNECTION_REQUEST: /*server won't send it*/
        case UDPMessageInterface::TypeId::USER_COMMANDS:
        case UDPMessageInterface::TypeId::EXIT_GAME:
        case UDPMessageInterface::TypeId::LOBBY_REQUEST:
        case UDPMessageInterface::TypeId::ROOM_LIST_REQUEST:
        case UDPMessageInterface::TypeId::KEEP_ALIVE:
        case UDPMessageInterface::TypeId::LAST_ONE:
            debug("processServerMessages: What the hell? typeId: %d", msg->typeId());
            break;
        }
    }

    if(!findGame && now_ms() - lastMessageArrivalTime_ > 1000) {
        if(!messageboxes_.size()) {
            TranslateInterface::Key message = TranslateInterface::Key::TRA__CONNECTING;
            auto messagebox = new Messagebox1(1e4, message);
            Application::showMessageBox(messagebox);
        }
        debug("WARNING processServerMessages: %f", now_ms() - lastMessageArrivalTime_);
        // maybe a reconnect would be in order
        lastMessageArrivalTime_ = now_ms();

        if(serverHost_.length() && serverPort_) {
            UDPMessageInterfaceUPtr newMsg;
            if(createConnection_) {
                newMsg = UDPMessageFactory::createReconnectionRequest(latestWorld()->multiplayer()->clientId());
            } else {
                newMsg = UDPMessageFactory::createReconnectionRequest(latestWorld()->multiplayer()->clientId());
            }
            UDP udp(serverHost_, serverPort_, true);
            [[maybe_unused]]bool xx = udp.sendMessage2Server(newMsg);
        }
    }
}

} // namespace Application
