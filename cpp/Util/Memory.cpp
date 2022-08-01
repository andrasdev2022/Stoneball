#include "Memory.h"
#include <memory>

namespace Memory {

template<>
const unsigned char* copy(std::string& dst, const unsigned char* src) noexcept {
    uint16_t strLength{};
    src = Memory::copy(strLength, src);
    std::unique_ptr<char[]> str = std::make_unique<char[]>(strLength + 1);
    str.get()[strLength] = 0;
    memcpy(str.get(), src, strLength);
    dst = str.get();
    return src + strLength;
}

template<>
const unsigned char* copy(std::wstring& dst, const unsigned char* src) noexcept {
    uint16_t strLength{};
    src = Memory::copy(strLength, src);
    std::unique_ptr<wchar_t[]> str = std::make_unique<wchar_t[]>(strLength + 1);
    str.get()[strLength] = 0;
    if constexpr (sizeof(wchar_t) == 4) {
        memcpy(str.get(), src, strLength * sizeof(wchar_t));
        dst = str.get();
        return src + strLength * sizeof(wchar_t);
    } else {
        int i = 0;
        while (strLength--) {
            memcpy(&str[i++], src, sizeof(wchar_t));
            src += 4;
        }
        dst = str.get();
        return src;
    }
}

} // namespace Memory
