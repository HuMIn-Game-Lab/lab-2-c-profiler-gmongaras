#include "time.hpp"
#include <chrono>

double GetCurrentTimeSeconds() {
    // Get the current time in seconds. It is the time since the program started.
    // Since static variables are only created once.
    static auto start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(now - start).count();
}

