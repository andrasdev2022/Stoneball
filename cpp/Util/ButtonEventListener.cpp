#include "ButtonEventListener.h"

std::string ButtonEventListener::str(ButtonState b) noexcept {
    switch(b) {
        case ButtonState::BS_UP: return "BS_UP"; break;
        case ButtonState::BS_DOWN: return "BS_DOWN"; break;
    }
    return "";
}
