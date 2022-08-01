#include "Zlib_.h"
#include <zlib.h>
#include "CommonUtil.h"

Zlib::Zlib() {

}

bool Zlib::inflateMsg(unsigned char* inBuff, size_t size) noexcept {
    z_stream_s stream{};
    stream.next_in = inBuff;
    stream.avail_in = size;
    stream.next_out = deflateBuffer_;
    stream.avail_out = deflateBufferSize_;
    stream.data_type = Z_BINARY;
    if(inflateInit(&stream) != Z_OK) {
        debug("ERROR: inflateInit");
        inflateEnd(&stream);
        return false;
    }

    while(stream.avail_in > 0) {
        int r = inflate(&stream, Z_FULL_FLUSH);
        if(r != Z_OK && r != Z_STREAM_END) {
            debug("ERROR: inflate 1, error code: %d, size: %u", r, size);
            inflateEnd(&stream);
            return false;
        }
        if(r == Z_STREAM_END) break;
    }

    if(inflate(&stream, Z_FINISH) != Z_STREAM_END) {
        debug("ERROR: inflateInit 2");
        inflateEnd(&stream);
        return false;
    }
    deflateOutSize_ = stream.total_out;
    if(inflateEnd(&stream) != Z_OK) {
        debug("ERROR: inflateEnd");
        return false;
    }
    return true;
}

bool Zlib::deflateMsg(const unsigned char* inBuff, size_t size) noexcept {
    z_stream_s stream{};
    stream.next_in = (unsigned char*)inBuff;
    stream.avail_in = size;
    stream.next_out = deflateBuffer_;
    stream.avail_out = deflateBufferSize_;
    stream.data_type = Z_BINARY;
    if(deflateInit(&stream, 5) != Z_OK) {
        debug("ERROR: deflateInit");
        deflateEnd(&stream);
        return false;
    }

    while(stream.avail_in > 0) {
        int r = deflate(&stream, Z_FULL_FLUSH);
        if(r != Z_OK && r != Z_STREAM_END) {
            debug("ERROR: deflate 1");
            deflateEnd(&stream);
            return false;
        }
        if(r == Z_STREAM_END) break;
    }

    if(deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        debug("ERROR: deflate 2");
        deflateEnd(&stream);
        return false;
    }
    deflateOutSize_ = stream.total_out;
    if(deflateEnd(&stream) != Z_OK) {
        debug("ERROR: deflateEnd");
        return false;
    }
    return true;
}

