#include "Serialize.h"
#include <string>

std::unique_ptr<unsigned char[]> Serialize::get() const noexcept {
    if(!size_ ) return nullptr;
    std::unique_ptr<unsigned char[]> retv = std::make_unique<unsigned char[]>(size_);
    memcpy(retv.get(), startPtr_.data(), size_);
    return retv;
}

size_t Serialize::addString(const std::string& obj) noexcept {
    uint16_t length = (uint16_t)obj.length();
    auto retv = add(length);
    add3(obj.c_str(), obj.length());
    return retv;
}

size_t Serialize::addString(const std::wstring& obj) noexcept {
    uint16_t length = (uint16_t)obj.length();
    auto retv = add(length);
    if constexpr (sizeof(wchar_t) == 4) {
        add3(obj.c_str(), obj.length() * sizeof(wchar_t));
    } else {
        std::unique_ptr<uint32_t[]> temp = std::make_unique<uint32_t[]>(length);
        for (uint16_t i = 0; const auto & x : obj) temp[i++] = x;
        add3(temp.get(), length * 4);
    }
    return retv;
}
