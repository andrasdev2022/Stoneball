#ifndef __IDGENERATOR_H__
#define __IDGENERATOR_H__

#include <inttypes.h>

class IdGenerator
{
public:
    static uint32_t currentId() noexcept;
    static uint32_t generateId() noexcept;
};

#endif // IDGENERATOR_H
