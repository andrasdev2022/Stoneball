#include "Application.h"
#include <sys/time.h>
#include <cstddef>

namespace Application {
    double now() noexcept {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000. + tv.tv_usec/1000.;
    }
}
