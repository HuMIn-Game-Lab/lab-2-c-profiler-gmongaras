#include "profiler.hpp"
#include "time.hpp"
#include <iostream>
#include <fstream>







Profiler* Profiler::gProfiler = nullptr;

TimeRecordStart::TimeRecordStart(char const* sectionName, double secondsAtStart): sectionName(sectionName), secondsAtStart(secondsAtStart) {}
TimeRecordStart::~TimeRecordStart() {}

TimeRecordStop::TimeRecordStop(char const* sectionName, double elapsedTime): sectionName(sectionName), elapsedTime(elapsedTime), lineNumber(0), fileName("null"), functionName("null") {}
TimeRecordStop::TimeRecordStop(char const* sectionName, double elapsedTime, int lineNumber, const char* fileName, const char* functionName): sectionName(sectionName), elapsedTime(elapsedTime), lineNumber(lineNumber), fileName(fileName), functionName(functionName) {}
TimeRecordStop::~TimeRecordStop() {}

ProfilerStats::ProfilerStats(char const* sectionName): sectionName(sectionName), count(0), totalTime(0), minTime(DBL_MAX), maxTime(0), avgTime(0), filename("null"), functionName("null"), lineNumber(0), timeline(std::vector<double>()) {}
ProfilerStats::~ProfilerStats() {}

ProfilerScopeObject::ProfilerScopeObject(char const* sectionName) {
    Profiler::GetInstance()->EnterSection(sectionName);
    this->sectionName = sectionName;
}

ProfilerScopeObject::~ProfilerScopeObject() {
    Profiler::GetInstance()->ExitSection(sectionName);
}






Profiler::Profiler() {
    gProfiler = this;

    // startTimes.reserve(100);
    // elapsedTimes.reserve(1000000);
}

Profiler* Profiler::GetInstance() {
    if (gProfiler == nullptr) {
        gProfiler = new Profiler();
    }
    return gProfiler;
}

Profiler::~Profiler() {
    for (auto& stat : stats) {
        delete stat.second;
    }
}

void Profiler::EnterSection(char const* sectionName) {
    // Does the map contain the sectionName?
    if (startTimes.find(sectionName) == startTimes.end()) {
        startTimes[sectionName] = std::vector<TimeRecordStart>();
    }

    double secondsAtStart = GetCurrentTimeSeconds();

    startTimes.at(sectionName).emplace_back(sectionName, secondsAtStart);
    // TimeRecordStart* start = new TimeRecordStart(sectionName, secondsAtStart);
    // startTimes.push_back(start);
}

void Profiler::ExitSection(char const* sectionName) {
    double secondsAtStop = GetCurrentTimeSeconds();

    // Get the last start time
    TimeRecordStart const& currentSection = startTimes.at(sectionName).back();

    #if defined( DEBUG_PROFIER )
        // Verify the stack isn't empty

        // Verify the currentSection matches the sectionName.
    #endif

    // Calculate the elapsed time
    double elapsedTime = secondsAtStop - currentSection.secondsAtStart;
    // ReportSectionTime(sectionName, elapsedTime);
    elapsedTimes.emplace_back(sectionName, elapsedTime);
    startTimes.at(sectionName).pop_back();
}

void Profiler::ExitSection(char const* sectionName, int lineNumber, const char* fileName, const char* functionName) {
    double secondsAtStop = GetCurrentTimeSeconds();

    // Get the last start time
    TimeRecordStart const& currentSection = startTimes.at(sectionName).back();

    #if defined( DEBUG_PROFIER )
        // Verify the stack isn't empty

        // Verify the currentSection matches the sectionName.
    #endif

    // Calculate the elapsed time
    double elapsedTime = secondsAtStop - currentSection.secondsAtStart;
    ReportSectionTime(sectionName, elapsedTime, lineNumber, fileName, functionName);
    // elapsedTimes.emplace_back(sectionName, elapsedTime);
    startTimes.at(sectionName).pop_back();
}

void Profiler::calculateStats() {
    // Clear the stats
    for (auto& stat : stats) {
        delete stat.second;
    }
    stats.clear();

    // Cumulative time for each section
    std::map<char const*, std::vector<double>> cumulativeTime;

    // Calculate the stats
    for (auto& elapsed : elapsedTimes) {
        // Does the map contain the sectionName?
        if (stats.find(elapsed.sectionName) == stats.end()) {
            stats[elapsed.sectionName] = new ProfilerStats(elapsed.sectionName);
        }

        // Stats for this section name
        ProfilerStats* stat = stats.at(elapsed.sectionName);
        // Total number of times this section was called
        stat->count++;
        // Total time of all calls to this section
        stat->totalTime += elapsed.elapsedTime;
        // Min and max time of this section
        stat->minTime = std::min(stat->minTime, elapsed.elapsedTime);
        stat->maxTime = std::max(stat->maxTime, elapsed.elapsedTime);

        // Filename for each section
        stat->filename = elapsed.fileName;
        stat->functionName = elapsed.functionName;
        stat->lineNumber = elapsed.lineNumber;

        // Add the time to the timeline
        if (cumulativeTime.find(elapsed.sectionName) == cumulativeTime.end()) {
            // If it doesn't exist, add the section name and current time
            cumulativeTime[elapsed.sectionName] = std::vector<double>();
            cumulativeTime[elapsed.sectionName].push_back(elapsed.elapsedTime);
        } else {
            // If it does exist, add the current time to the timeline
            cumulativeTime[elapsed.sectionName].push_back(cumulativeTime[elapsed.sectionName].back() + elapsed.elapsedTime);
        }
    }

    // Save all the cumulative times
    for (auto& stat : stats) {
        ProfilerStats* stat_ = stat.second;
        stat_->timeline = cumulativeTime[stat_->sectionName];
    }

    // Calculate the average times
    for (auto& stat : stats) {
        ProfilerStats* stat_ = stat.second;
        stat_->avgTime = stat_->totalTime / stat_->count;
    }
}

// Calculate statistics for a specific section and return them
ProfilerStats Profiler::calculateStats(char const* sectionName) {
    // Calculate the stats
    calculateStats();

    // Return the stats for the section
    return *stats.at(sectionName);
}


void Profiler::printStats() {
    // Calculate the stats
    calculateStats();

    // Print out the stats
    for (auto& stat : stats) {
        ProfilerStats* stat_ = stat.second;
        std::cout << "Section Name: " << stat_->sectionName << "\n";
        std::cout << "Count: " << stat_->count << "\n";
        std::cout << "Total Time: " << stat_->totalTime << "\n";
        std::cout << "Min Time: " << stat_->minTime << "\n";
        std::cout << "Max Time: " << stat_->maxTime << "\n";
        std::cout << "Avg Time: " << stat_->avgTime << "\n";
        std::cout << "Filename: " << stat_->filename << "\n";
        std::cout << "Function Name: " << stat_->functionName << "\n";
        std::cout << "Line Number: " << stat_->lineNumber << "\n";
        std::cout << "\n";
    }
}

void Profiler::reset() {
    // Clear the stats
    for (auto& stat : stats) {
        delete stat.second;
    }
    stats.clear();

    // Clear the elapsed times
    elapsedTimes.clear();

    // Clear the start times
    startTimes.clear();
}

void Profiler::saveStatsToCSV(const char* filename) {
    // Calculate the stats
    calculateStats();

    // Open the file
    std::ofstream file;
    file.open(filename);

    // Header
    file << "Section Name,";
    file << "Count,";
    file << "Total Time,";
    file << "Min Time,";
    file << "Max Time,";
    file << "Avg Time,";
    file << "Filename,";
    file << "Function Name,";
    file << "Line Number,";


    // Write the stats to the file
    for (auto& stat : stats) {
        ProfilerStats* stat_ = stat.second;
        file << stat_->sectionName << ",";
        file << stat_->count << ",";
        file << stat_->totalTime << ",";
        file << stat_->minTime << ",";
        file << stat_->maxTime << ",";
        file << stat_->avgTime << ",";
        file << stat_->filename << ",";
        file << stat_->functionName << ",";
        file << stat_->lineNumber << ",";
        file << "\n";
    }

    // Close the file
    file.close();
}

void Profiler::saveStatsToJSON(const char* filename) {
    // Calculate the stats
    calculateStats();

    // Open the file
    std::ofstream file;
    file.open(filename);

    // Header
    file << "{\n";
    file << "  \"profiler\": [\n";

    // Write the stats to the file
    // Skip the last comma
    int count = 0;
    for (auto& stat : stats) {
        ProfilerStats* stat_ = stat.second;
        file << "    {\n";
        file << "      \"Section Name\": \"" << stat_->sectionName << "\",\n";
        file << "      \"Count\": " << stat_->count << ",\n";
        file << "      \"Total Time\": " << stat_->totalTime << ",\n";
        file << "      \"Min Time\": " << stat_->minTime << ",\n";
        file << "      \"Max Time\": " << stat_->maxTime << ",\n";
        file << "      \"Avg Time\": " << stat_->avgTime << ",\n";
        file << "      \"Filename\": \"" << stat_->filename << "\",\n";
        file << "      \"Function Name\": \"" << stat_->functionName << "\",\n";
        file << "      \"Line Number\": " << stat_->lineNumber << ",\n";
        file << "      \"Timeline\": [";
        for (int i = 0; i < stat_->timeline.size(); i++) {
            file << stat_->timeline[i];
            if (i < stat_->timeline.size() - 1) {
                file << ", ";
            }
        }
        file << "]\n";
        file << "    }";
        count++;
        if (count < stats.size()) {
            file << ",";
        }
        file << "\n";
    }

    // Close the file
    file << "  ]\n";
    file << "}\n";
    file.close();
}

void Profiler::ReportSectionTime(char const* sectionName, double elapsedTime) {
    elapsedTimes.emplace_back(sectionName, elapsedTime);
}

void Profiler::ReportSectionTime(char const* sectionName, double elapsedTime, int lineNumber, const char* fileName, const char* functionName) {
    elapsedTimes.emplace_back(sectionName, elapsedTime, lineNumber, fileName, functionName);
}