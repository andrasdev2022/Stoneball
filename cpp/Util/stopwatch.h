#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include <unordered_map>
#include <chrono>
#include <string>

using namespace std::chrono;

class StopWatch {
public:
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_point;
    typedef std::pair<time_point, size_t> StopWatchRec;

    void start(std::string key) noexcept;
    size_t timeMs(std::string key) noexcept; // microseconds
    
    // for duration measuring
    void saveDuration(std::string key) noexcept;
    size_t duration(std::string key) noexcept;
    void reset() noexcept {clocks.clear();}
    
private:
    std::unordered_map<std::string, StopWatchRec> clocks;
};

// usage
// start - timeMs pair
// or in subrutines
// start - saveDuration - duration (summary of elapsed time in subrutine)

//StopWatch st;

#endif // __STOPWATCH_H__
