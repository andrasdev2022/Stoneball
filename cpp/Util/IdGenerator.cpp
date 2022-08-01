#include "IdGenerator.h"
#include "Defines.h"

namespace {
    THREAD_LOCAL uint32_t idGenerator = 0;
}

uint32_t IdGenerator::currentId() noexcept {
    return idGenerator;
}

uint32_t IdGenerator::generateId() noexcept {
    return ++idGenerator;
}
