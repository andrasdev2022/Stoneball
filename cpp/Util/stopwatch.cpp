#include "stopwatch.h"

void StopWatch::start(std::string key) noexcept {
    auto it = clocks.find(key);
    if(it == clocks.end()) {
        clocks.emplace(key, StopWatchRec(high_resolution_clock::now(), 0));
    } else {
        it->second.first = high_resolution_clock::now();
    }
}

size_t StopWatch::timeMs(std::string key) noexcept {
    auto s = clocks.find(key);
    if(s != clocks.end()) {
        auto t = high_resolution_clock::now();
        return duration_cast<microseconds>(t - s->second.first).count();
    }
    return 0;
}

void StopWatch::saveDuration(std::string key) noexcept {
    auto s = clocks.find(key);

    if(s != clocks.end()) {
        auto t = high_resolution_clock::now();
        s->second.second += duration_cast<microseconds>(t - s->second.first).count();
    }
}

size_t StopWatch::duration(std::string key) noexcept {
    auto s = clocks.find(key);
    if(s != clocks.end()) {
        return s->second.second;
    }
    return 0;
}
