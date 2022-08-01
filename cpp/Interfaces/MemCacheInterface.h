#ifndef __MEMCACHEINTERFACE_H__
#define __MEMCACHEINTERFACE_H__

#include <vector>
#include "CommonUtil.h"
#include "AnyCastItemInterface.h"
#include <iostream>

template<typename DATA>
class MemCacheInterface {
public:
    virtual ~MemCacheInterface(){}

    virtual void saveWindow(const DATA* src, int stride, int width, int height) noexcept = 0;
    virtual void copyWindow(DATA* dst) noexcept = 0;
    virtual void makeBuffer(int stride, int width, int height) noexcept = 0;
    virtual DATA* ptr() noexcept = 0;
    virtual bool ready() const noexcept = 0;
    virtual void setReady(bool ready) noexcept = 0;

    MemCacheInterface() {
    }

    MemCacheInterface(const MemCacheInterface& o) = delete;

    template<typename... T>
    void saveParameters(T&&... params) noexcept {
        paramCount_ = sizeof...(params);
        parameters_ = std::make_unique<std::unique_ptr<AnyCastItemInterface>[]>(paramCount_);
        std::size_t n{0};
        (parameters_[n++].reset(new AnyCastItem<std::decay_t<T>>(std::forward<T>(params))), ...);
    }

    template<typename... T>
    bool parameterCheck(T&&... params) noexcept {
        if(paramCount_ != sizeof...(params)) return false;
        std::size_t n{0};
        return (parameterCheck1(n++, params) &&  ...);
    }

    size_t size() const noexcept {return paramCount_;}

private:

    template<typename T>
    bool parameterCheck1(int index, T&& v) noexcept {
        switch(parameters_[index]->type()) {
        case AnyCastItemInterface::Type::T_FLOAT: {
            return parameters_[index]->cast<float>()->equalf(v);
        }break;
        case AnyCastItemInterface::Type::T_DOUBLE: {
            return parameters_[index]->cast<double>()->equalf(v);
        }break;
        case AnyCastItemInterface::Type::T_LONG_DOUBLE: {
            return parameters_[index]->cast<long double>()->equalf(v);
        }break;
        case AnyCastItemInterface::Type::T_INT8: {
            return parameters_[index]->cast<int8_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_INT16: {
            return parameters_[index]->cast<int16_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_INT32: {
            return parameters_[index]->cast<int32_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_INT64: {
            return parameters_[index]->cast<int64_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_UINT8: {
            return parameters_[index]->cast<uint8_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_UINT16: {
            return parameters_[index]->cast<uint16_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_UINT32: {
            return parameters_[index]->cast<uint32_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_UINT64: {
            return parameters_[index]->cast<uint64_t>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_STRING: {
            return parameters_[index]->cast<std::string>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_WSTRING: {
            return parameters_[index]->cast<std::wstring>()->equal(v);
        }break;
        case AnyCastItemInterface::Type::T_UNKNOWN: {
            using DT1 = std::decay_t<decltype(v)>;
            if(auto p = parameters_[index]->cast<DT1>()) {
                return p->equal(v);
            }
        }break;
        }
        return false;
    }

    std::unique_ptr<std::unique_ptr<AnyCastItemInterface>[]> parameters_;
    size_t paramCount_{};
};

using MemCacheInterfaceUInt16 = std::unique_ptr<MemCacheInterface<uint16_t>>;
#endif // __MEMCACHEINTERFACE_H__
