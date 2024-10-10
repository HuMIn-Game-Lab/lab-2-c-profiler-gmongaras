#include "profiler.hpp"
#include "time.hpp"
#include <iostream>

Profiler* Profiler::gProfiler = nullptr;

TimeRecordStart::TimeRecordStart(char const* sectionName, double secondsAtStart): sectionName(sectionName), secondsAtStart(secondsAtStart) {}
TimeRecordStart::~TimeRecordStart() {}

TimeRecordStop::TimeRecordStop(char const* sectionName, double elapsedTime): sectionName(sectionName), elapsedTime(elapsedTime), lineNumber(0), fileName("null"), functionName("null") {}
TimeRecordStop::TimeRecordStop(char const* sectionName, double elapsedTime, int lineNumber, const char* fileName, const char* functionName): sectionName(sectionName), elapsedTime(elapsedTime), lineNumber(lineNumber), fileName(fileName), functionName(functionName) {}
TimeRecordStop::~TimeRecordStop() {}

ProfilerStats::ProfilerStats(char const* sectionName): sectionName(sectionName), count(0), totalTime(0), minTime(DBL_MAX), maxTime(0), avgTime(0), filename("null"), functionName("null"), lineNumber(0) {}
ProfilerStats::~ProfilerStats() {}

Profiler::Profiler() {
    gProfiler = this;

    startTimes.reserve(100);
    elapsedTimes.reserve(1000000);
}

Profiler::~Profiler() {
    for (auto& stat : stats) {
        delete stat.second;
    }
}

void Profiler::EnterSection(char const* sectionName) {
    double secondsAtStart = GetCurrentTimeSeconds();

    startTimes.emplace_back(sectionName, secondsAtStart);
    // TimeRecordStart* start = new TimeRecordStart(sectionName, secondsAtStart);
    // startTimes.push_back(start);
}

void Profiler::ExitSection(char const* sectionName) {
    double secondsAtStop = GetCurrentTimeSeconds();

    // Get the last start time
    TimeRecordStart const& currentSection = startTimes.back();

    #if defined( DEBUG_PROFIER )
        // Verify the stack isn't empty

        // Verify the currentSection matches the sectionName.
    #endif

    // Calculate the elapsed time
    double elapsedTime = secondsAtStop - currentSection.secondsAtStart;
    ReportSectionTime(sectionName, elapsedTime);
    // elapsedTimes.emplace_back(sectionName, elapsedTime);
    startTimes.pop_back();
}

void Profiler::ReportSectionTime(char const* sectionName, double elapsedTime) {
    elapsedTimes.emplace_back(sectionName, elapsedTime);
}