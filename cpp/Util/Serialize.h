#ifndef __SERIALIZE_H__
#define __SERIALIZE_H__

#include <string.h>
#include <memory>
#include <string>
#include <vector>

class Serialize {
public:
    static constexpr size_t SERIALIZE_ERROR = 0xffffffff;
    Serialize() noexcept = default;
    Serialize(const Serialize&) = delete;
    Serialize(Serialize&&) = delete;
    Serialize& operator=(const Serialize&) = delete;
    Serialize& operator=(Serialize&&) = delete;

    template<typename T>
    size_t add(const T& obj) noexcept {
        size_t newSize = size_ + sizeof(obj);
        startPtr_.resize(newSize);
        memcpy(startPtr_.data() + size_, &obj, sizeof(obj));
        size_t retV = size_;
        size_ = newSize;
        return retV;
    }

    size_t add2(const Serialize& obj) noexcept {
        if(!obj.size()) {return SERIALIZE_ERROR;}
        size_t newSize = size_ + obj.size();
        startPtr_.resize(newSize);
        memcpy(startPtr_.data() + size_, obj.startPtr_.data(), obj.size());
        size_t retV = size_;
        size_ = newSize;
        return retV;
    }

    template<typename T>
    size_t add3(const T& obj, size_t size) noexcept {
        if(!size) {return SERIALIZE_ERROR;}
        size_t newSize = size_ + size;
        startPtr_.resize(newSize);
        memcpy(startPtr_.data() + size_, obj, size);
        size_t retV = size_;
        size_ = newSize;
        return retV;
    }

    size_t addString(const std::string& obj) noexcept;
    size_t addString(const std::wstring& obj) noexcept;

    template<typename T>
    size_t replace(const T& obj, size_t position) noexcept {
        if(position + sizeof(obj) > size_) return SERIALIZE_ERROR;
        memcpy(startPtr_.data() + position, &obj, sizeof(obj));
        return position;
    }

    size_t size() const noexcept {return size_;}
    std::unique_ptr<unsigned char[]> get() const noexcept;

private:
    std::vector<char> startPtr_;
    size_t size_{0};
};
#endif // __SERIALIZE_H__
