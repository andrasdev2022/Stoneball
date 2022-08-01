#include "android.h"
#include <stdint.h>
#include <time.h>
#ifndef WIN32
	#include <sys/time.h>
#else
    #include "UtilWinDefs.h"
#endif
#include "Global.h"
#include <string>
#include <stdio.h>
#include <memory>
#include <stdarg.h>
#include <mutex>
#include "SoundEffect.h"
#ifndef ANDROID_NDK
#include <QDesktopServices>
#include <QUrl>
#ifndef WIN32
	#include <execinfo.h>
#endif
#else
#include <iostream>
#include <iomanip>
#include <unwind.h>
#include <dlfcn.h>
#include <sstream>
#endif
#include "NTPClient.h"
#include "CommonUtil.h"

namespace {
    double convertTime2Ms(const struct timeval& tv) {
        return tv.tv_sec * 1000. + tv.tv_usec/1000.;
    }
    static double now_{};
    static double lastNow_{};
}

/* Return current time in milliseconds */
double now_ms(void) {
    if(now_ < 1e-7) {
        debug("Init now");
        struct timeval tv;
        gettimeofday(&tv, NULL);
        now_ = convertTime2Ms(tv);
        lastNow_ = now_;
    }
    return now_;
}

void update_now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double now = convertTime2Ms(tv);
    if(now - lastNow_ > 50.) {
        // if the device play with 20 or less fps, we simulate the 20 for the game in order
        // to prevent that the hero walk around
        now_ += 50.;
    } else {
        now_ += now - lastNow_;
    }
    lastNow_ = now;
}

#ifndef WIN32

#ifndef ANDROID_NDK

void show_backtrace() {
    debug("show_backtrace():");
    void* callstack[128];
    int i, frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
    for(i = 0; i < frames; ++i) {
        //printf("%s\n", strs[i]);
        debug("%s\n", strs[i]);
    }
    free(strs);
}

#else
namespace {

struct BacktraceState
{
    void** current;
    void** end;
};

static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context* context, void* arg)
{
    BacktraceState* state = static_cast<BacktraceState*>(arg);
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        } else {
            *state->current++ = reinterpret_cast<void*>(pc);
        }
    }
    return _URC_NO_REASON;
}

} // namespace

size_t captureBacktrace(void** buffer, size_t max)
{
    BacktraceState state = {buffer, buffer + max};
    _Unwind_Backtrace(unwindCallback, &state);

    return state.current - buffer;
}

void dumpBacktrace(std::ostream& os, void** buffer, size_t count)
{
    for (size_t idx = 0; idx < count; ++idx) {
        const void* addr = buffer[idx];
        const char* symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname) {
            symbol = info.dli_sname;
        }

        os << "  #" << std::setw(2) << idx << ": " << addr << "  " << symbol << "\n";
    }
}

void show_backtrace() {
    const size_t max = 30;
    void* buffer[max];
    std::ostringstream oss;

    dumpBacktrace(oss, buffer, captureBacktrace(buffer, max));
    debug("%s", oss.str().c_str());
}

#endif
#endif // ifndef WIN32

void openPrivacyPolicy() {
#ifndef ANDROID_NDK
    QDesktopServices::openUrl(QUrl("https://andrasdev2022.github.io"));
#else
    javaOpenWebBrowser("https://andrasdev2022.github.io");
#endif
}
