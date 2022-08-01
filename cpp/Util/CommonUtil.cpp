#include "CommonUtil.h"
#include <string>
#include <memory>
#include <stdarg.h>
#include <mutex>

//namespace CommonUtil {


namespace {
    std::string logFilePath;
    int logFileDescriptor = 0;
    void close_file(FILE* fp) {if(fp) fclose(fp);}
    std::unique_ptr<FILE, decltype(&close_file)> file(0, &close_file);
    std::mutex mutex_;
}

#ifdef __cplusplus
extern "C" {
#endif

void setLogFilePath(const char* path) {
    logFilePath = path;
}

void setLogFileDescriptor(int fd) {
    logFileDescriptor = fd;
}

void Unused(char*, ...){}

void debug(const char* format, ...) {
    std::unique_lock<std::mutex> lck(mutex_);
    if(logFilePath.length() > 0 && !file) {
        file = std::unique_ptr<FILE, decltype(&close_file)>(fopen(logFilePath.c_str(), "a+t"), &close_file);
    } else if(logFileDescriptor > 0 && !file) {
        file = std::unique_ptr<FILE, decltype(&close_file)>(fdopen(logFileDescriptor, "a+t"), &close_file);
    } else {
        // This should happen only on PC
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        printf("\n");
        fflush(stdout);
        va_end(args);
    }
    if(file) {
        va_list args;
        va_start(args, format);
        vfprintf(file.get(), format, args);
        va_end(args);
        fprintf(file.get(), "\n");
        fflush(file.get());
        //fclose(file);
    }
}

#ifdef __cplusplus
}
#endif

//} // namespace CommonUtil
