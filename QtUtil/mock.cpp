#include "mock.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

namespace MOCK {
    bool android_log_print_enabled = true;
}

void __android_log_print(const char* format,...) {
    if(MOCK::android_log_print_enabled) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    }
}

namespace MOCK {
    void logoff() {
        android_log_print_enabled = false;
    }

    void logon() {
        android_log_print_enabled = true;
    }

} // namespace MOCK

char callProfileActivity = 'n';
char callAboutActivity = 'n';
