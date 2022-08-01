#ifndef __MEMCACHE_H__
#define __MEMCACHE_H__

#include "MemCacheInterface.h"

class MemCache : public MemCacheInterface<uint16_t>
{
public:
    MemCache();
    MemCache(const MemCache&) = delete;

    void saveWindow(const uint16_t* src, int stride, int width, int height) noexcept override;
    void copyWindow(uint16_t* dst) noexcept override;
    void makeBuffer(int stride, int width, int height) noexcept override;
    uint16_t* ptr() noexcept override {return ptr_.get();}
    virtual bool ready() const noexcept override {return ready_;}
    virtual void setReady(bool ready) noexcept override {ready_ = ready;}

private:
    int width_{};
    int height_{};
    int stride_{};
    bool ready_{};
    std::unique_ptr<uint16_t[]> ptr_;
};

#endif // MEMCACHE_H
