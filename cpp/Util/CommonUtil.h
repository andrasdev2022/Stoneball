#ifndef __COMMONUTIL_H__
#define __COMMONUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

//namespace CommonUtil {

    void Unused(char*, ...);
    void debug(const char* format, ...);
    void setLogFilePath(const char* path);
    void setLogFileDescriptor(int fd);

//} // namespace CommonUtil


#ifdef __cplusplus
}
#endif

#endif // __COMMONUTIL_H__
