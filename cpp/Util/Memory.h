#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <string.h>
#include <string>

namespace Memory {

template<typename T>
const unsigned char* copy(T& dst, const unsigned char* src) noexcept {
    memcpy(&dst, src, sizeof(T));
    return src + sizeof(T);
}

template<>
const unsigned char* copy(std::string& dst, const unsigned char* src) noexcept;

template<>
const unsigned char* copy(std::wstring& dst, const unsigned char* src) noexcept;

template<typename T>
void memset(T* dst, T value, size_t length) {
    uint64_t buffer1;
    unsigned char* buffer1Ptr = (unsigned char*)&buffer1;

    if(sizeof(T) == 4) {
        memcpy(&buffer1, &value, sizeof(T));
        memcpy(buffer1Ptr + 4, &value, sizeof(T));
        while(length >= 2) {
            memcpy(dst, &buffer1, 8);
            length -= 2;
            dst += 2;
        }
        if(length) {
            memcpy(dst, &value, 4);
        }
    } else if(sizeof(T) == 2) {
        memcpy(&buffer1, &value, sizeof(T));
        memcpy(buffer1Ptr + 2, &value, sizeof(T));
        memcpy(buffer1Ptr + 4, &value, sizeof(T));
        memcpy(buffer1Ptr + 6, &value, sizeof(T));
        while(length >= 4) {
            memcpy(dst, &buffer1, 8);
            length -= 4;
            dst += 4;
        }
        while(length) {
            memcpy(dst++, &value, 2);
            length--;
        }
    } else if(sizeof(T) == 1) {
        memset(dst, value, length);
    } else {
        while(length) {
            memcpy(dst++, &value, sizeof(value));
            length--;
        }
    }

}

} // namespace Memory

#endif // __MEMORY_H__
