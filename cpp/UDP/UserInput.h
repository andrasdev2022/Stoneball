#ifndef __USERINPUT_H__
#define __USERINPUT_H__

#include "UDP.h"
class UserInput : public UDP::Message {
public:
    size_t length() const noexcept override;
    const char* payload() noexcept override;
};
#endif // #ifndef __USERINPUT_H__
