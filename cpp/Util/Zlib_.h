#ifndef __ZLIB_H__
#define __ZLIB_H__

#include <stdlib.h>
#include <memory>

class Zlib
{
public:
    Zlib();
    bool inflateMsg(unsigned char* inBuff, size_t size) noexcept;
    bool deflateMsg(const unsigned char* inBuff, size_t size) noexcept;

    size_t deflateOutSize() const noexcept {return deflateOutSize_;}
    const unsigned char* deflateBuffer() const noexcept {return deflateBuffer_;}

private:
    static constexpr size_t deflateBufferSize_ = 10240;
    unsigned char deflateBuffer_[deflateBufferSize_];
    size_t deflateOutSize_;
};

using ZlibUPtr = std::unique_ptr<Zlib>;

#endif // ZLIBH_H
