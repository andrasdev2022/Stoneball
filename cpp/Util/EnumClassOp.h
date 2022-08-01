#ifndef __ENUMCLASSOP_H__
#define __ENUMCLASSOP_H__

template<typename T>
class EnumClassOp {
public:
    EnumClassOp(){}
    EnumClassOp(T val) : val_(static_cast<int>(val)) {}
    EnumClassOp& operator++() {++val_; return *this;}
    operator T() {return static_cast<T>(val_);}
    EnumClassOp operator %(T v) {
        EnumClassOp r;
        r.val_ = (val_ % static_cast<int>(v));
        return r;
    }
    operator int() const {return val_;}
    EnumClassOp& operator=(T val) {val_ = static_cast<int>(val); return *this;}
    EnumClassOp& operator=(int val) {val_ = val; return *this;}
    EnumClassOp operator+(int val) {EnumClassOp<T> retval;retval.val_ = val_ + val; return retval;}
    bool operator<=(const EnumClassOp& op) {return val_ <= op.val_;}
    bool operator==(T val) {return static_cast<T>(val_) == val;}
    bool operator<(T val) {return val_ < static_cast<int>(val);}
    bool operator<(const EnumClassOp& op) { return val_ < op.val_; }
    bool operator<=(T val) {return val_ <= static_cast<int>(val);}
    //EnumClassOp& operator=(const EnumClassOp& v) {if(this == &v) return *this; val_ = v.val_; return *this;}

private:
    int val_{};
};

#endif // __ENUMCLASSOP_H__
