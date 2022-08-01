#ifndef __ANYCASTITEMINTERFACE_H__
#define __ANYCASTITEMINTERFACE_H__

#include <memory>
#include <string>

template<typename T>
class AnyCastItem;

class AnyCastItemInterface {
public:
    enum class Type {
      T_FLOAT, T_DOUBLE, T_LONG_DOUBLE, T_INT8, T_INT16, T_INT32, T_INT64, T_UINT8, T_UINT16, T_UINT32, T_UINT64, T_STRING,
      T_WSTRING, T_UNKNOWN
    };
    virtual ~AnyCastItemInterface() = default;
    virtual Type type() const noexcept = 0;

    template<typename T>
    AnyCastItem<T>* cast() noexcept {return dynamic_cast<AnyCastItem<T>*>(this);}
};

template<typename T>
class AnyCastItem : public AnyCastItemInterface {
public:
    AnyCastItem(T v) : value_(v) {
        using DT = std::decay_t<decltype(v)>;
        if constexpr (std::is_same<DT, float>()) {
            type_ = Type::T_FLOAT;
        } else if constexpr (std::is_same<DT, double>()) {
            type_ = Type::T_DOUBLE;
        } else if constexpr (std::is_same<DT, long double>()) {
            type_ = Type::T_LONG_DOUBLE;
        } else if constexpr (std::is_same<DT, int8_t>()) {
            type_ = Type::T_INT8;
        } else if constexpr (std::is_same<DT, int16_t>()) {
            type_ = Type::T_INT16;
        } else if constexpr (std::is_same<DT, int32_t>()) {
            type_ = Type::T_INT32;
        } else if constexpr (std::is_same<DT, int64_t>()) {
            type_ = Type::T_UINT8;
        } else if constexpr (std::is_same<DT, uint8_t>()) {
            type_ = Type::T_UINT8;
        } else if constexpr (std::is_same<DT, uint16_t>()) {
            type_ = Type::T_UINT16;
        } else if constexpr (std::is_same<DT, uint32_t>()) {
            type_ = Type::T_UINT32;
        } else if constexpr (std::is_same<DT, uint64_t>()) {
            type_ = Type::T_UINT64;
        } else if constexpr (std::is_same<DT, std::string>()) {
            type_ = Type::T_STRING;
        } else if constexpr (std::is_same<DT, std::wstring>()) {
            type_ = Type::T_WSTRING;
        }
    }

    Type type() const noexcept override {return type_;}
    T value() const noexcept {return value_;}

    template<typename V>
    bool equal(V&& v) const noexcept {
        using DT1 = std::decay_t<decltype(v)>;
        using DT2 = std::decay_t<decltype(value_)>;
        if constexpr (std::is_same<DT1, DT2>()) {
            return v == value_;
        }
        return false;
    }

    template<typename V>
    bool equalf(V&& v) const noexcept {
        using DT1 = std::decay_t<decltype(v)>;
        using DT2 = std::decay_t<decltype(value_)>;
        if constexpr (std::is_same<DT1, DT2>()) {
            return absf(v - value_) < 1e-7;
        }
        return false;
    }

    template<typename V>
    static V absf(V&& v) noexcept {
        if(v >= 0.) return v;
        else return -v;
    }
protected:
    T value_;
    Type type_{Type::T_UNKNOWN};
};

#endif // __ANYCASTITEMINTERFACE_H__
