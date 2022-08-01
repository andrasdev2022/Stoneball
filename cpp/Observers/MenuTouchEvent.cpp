#include "MenuTouchEvent.h"
#include "ObserverTouchEventSubject.h"
#include "Application.h"
#include "UDP.h"
#include "UDPMessageFactory.h"
#include "Knight.h"
#include "Messagebox1.h"

bool MenuTouchEvent::notify(ObserverTouchEventSubject* subject) noexcept {
    Unused(0, subject);
    if(subject->action() == GameObject::Event::ACTION_MOVE) return false;

    //std::cout << "MenuTouchEvent::notify\n";
    // onTouchEvent(subject->action(), subject->x(), subject->y())
    if(subject->action() == GameObject::ACTION_UP) {
        return Application::activeMenuItem()->notify(subject);
    }
    return false;
}


void MenuTouchEvent::sendRoomListRequest() noexcept {
    UDPMessageInterfaceUPtr msg = UDPMessageFactory::createRoomListRequest(L"");
    UDP udp(loadBalancerHost, loadBalancerPort, true);
    TranslateInterface::Key message = TranslateInterface::Key::TRA__CONNECTING;
    if(!udp.sendMessage2Server(msg)) {
        message = TranslateInterface::Key::TRA__NETWORK_ERROR;
    }

    // Let's pop up a messagebox
    
    auto messagebox = new Messagebox1(1e6, message);
    Application::showMessageBox(messagebox);
    Application::destroyWidgets();
    Application::clearAvatarSelection();
}
