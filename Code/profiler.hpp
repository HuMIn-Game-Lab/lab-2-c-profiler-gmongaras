#pragma once
#include <cfloat>
#include <vector>
#include <map>


class TimeRecordStart {
public:
    TimeRecordStart(char const* sectionName, double secondsAtStart);
    ~TimeRecordStart();

    char const* sectionName;
    double secondsAtStart;
};

class TimeRecordStop {
public:
TimeRecordStop(char const* sectionName, double elapsedTime);
    TimeRecordStop(char const* sectionName, double elapsedTime, int lineNumber, const char* fileName, const char* functionName);
    ~TimeRecordStop();

    char const* sectionName;
    double elapsedTime;
    int lineNumber;
    const char* fileName;
    const char* functionName;
};

class ProfilerStats {
public:
    ProfilerStats(char const* sectionName);
    ~ProfilerStats();

    char const* sectionName;
    int count;
    double totalTime;
    double minTime;
    double maxTime;
    double avgTime;
    const char* filename;
    const char* functionName;
    int lineNumber;
};

class Profiler {
public:
    Profiler();
    ~Profiler();

    void EnterSection(char const* sectionName);
    void ExitSection(char const* sectionName);
    void calculateStats();
    void printStats();
    void printStatsToCSV(const char* filename);

    static Profiler* gProfiler;

private:
    void ReportSectionTime(char const* sectionName, double elapsedTime);
    void ReportSectionTime(char const* sectionName, double elapsedTime, int lineNumber, const char* fileName, const char* functionName);
    std::map<char const*, ProfilerStats*> stats;
    std::vector<TimeRecordStart> startTimes;
    std::vector<TimeRecordStop> elapsedTimes;
};