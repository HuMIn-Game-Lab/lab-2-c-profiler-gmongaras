#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <math.h>
#include <vector>
#include <execution>
#include "profiler.hpp"

constexpr float DEGREES_TO_RADIANS = (3.1415926535f / 180.0f);

Profiler* profiler = nullptr;

void Test1() {

    profiler->EnterSection("Trig Speed Test");
    
    constexpr int TRIG_TEST_NUM_ENTRIES = 100000;
    float randomYawDegreeTable[TRIG_TEST_NUM_ENTRIES] = {};

    profiler->EnterSection("Random Angle Generation");
    for (int i = 0; i < TRIG_TEST_NUM_ENTRIES; i++) {
        randomYawDegreeTable[i] = 360.f * float(rand()) / float(RAND_MAX);
    }
    profiler->ExitSection("Random Angle Generation");

    float biggestSoFar = 0;

    profiler->EnterSection("Total Cos and Sin Compute");
    for (int i = 0; i < TRIG_TEST_NUM_ENTRIES; i++) {
        float yawDegrees = randomYawDegreeTable[i];
        float cosDegrees = cosf(yawDegrees * DEGREES_TO_RADIANS);
        float sinDegrees = sinf(yawDegrees * DEGREES_TO_RADIANS);

        profiler->EnterSection("Cos and Sin Compute");
        if (cosDegrees + sinDegrees > biggestSoFar) {
            biggestSoFar = cosDegrees + sinDegrees;
        }
        profiler->ExitSection("Cos and Sin Compute");
    }
    profiler->ExitSection("Total Cos and Sin Compute");
    profiler->ExitSection("Trig Speed Test");

    std::cout << "Biggest cos+sin = " << biggestSoFar << std::endl;
}

void Test2() {
    // Class used to only time the scope of this function
    ProfilerScopeObject Test2Time("Test 2 Speed Test");


    PROFILER_ENTER("Trig Speed Test");
    
    constexpr int TRIG_TEST_NUM_ENTRIES = 100000;
    float randomYawDegreeTable[TRIG_TEST_NUM_ENTRIES] = {};

    PROFILER_ENTER("Random Angle Generation");
    for (int i = 0; i < TRIG_TEST_NUM_ENTRIES; i++) {
        randomYawDegreeTable[i] = 360.f * float(rand()) / float(RAND_MAX);
    }
    PROFILER_EXIT("Random Angle Generation");

    float biggestSoFar = 0;

    PROFILER_ENTER("Total Cos and Sin Compute");
    for (int i = 0; i < TRIG_TEST_NUM_ENTRIES; i++) {
        float yawDegrees = randomYawDegreeTable[i];
        float cosDegrees = cosf(yawDegrees * DEGREES_TO_RADIANS);
        float sinDegrees = sinf(yawDegrees * DEGREES_TO_RADIANS);

        PROFILER_ENTER("Cos and Sin Compute");
        if (cosDegrees + sinDegrees > biggestSoFar) {
            biggestSoFar = cosDegrees + sinDegrees;
        }
        PROFILER_EXIT("Cos and Sin Compute");
    }
    PROFILER_EXIT("Total Cos and Sin Compute");
    PROFILER_EXIT("Trig Speed Test");

    std::cout << "Biggest cos+sin = " << biggestSoFar << std::endl;
}

void Test3() {
    PROFILER_ENTER("Test 3 - interleave A"); // Enter section A
    int a = 0;
    for (int i = 0; i < 1000000; i++) {
        a += i;
    }
    PROFILER_ENTER("Test 3 - interleave B"); // Enter section B
    int b = 0;
    for (int i = 0; i < 1000000; i++) {
        b += i;
    }
    PROFILER_EXIT("Test 3 - interleave A"); // Exit section A
    int c = 0;
    for (int i = 0; i < 1000000; i++) {
        c += i;
    }
    PROFILER_EXIT("Test 3 - interleave B"); // Exit section B
}















constexpr int SORT_TEST_NUM_ENTRIES = 100000;

// Custom QuickSort implementation
void QuickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        QuickSort(arr, low, pi - 1);
        QuickSort(arr, pi + 1, high);
    }
}

void Test4_Original() {
    PROFILER_ENTER("QuickSort Test - Original");

    std::vector<int> data(SORT_TEST_NUM_ENTRIES);
    PROFILER_ENTER("Data Generation - Original");
    for (int& num : data) {
        num = rand() % SORT_TEST_NUM_ENTRIES;
    }
    PROFILER_EXIT("Data Generation - Original");

    PROFILER_ENTER("QuickSort Execution - Original");
    QuickSort(data, 0, SORT_TEST_NUM_ENTRIES - 1);
    PROFILER_EXIT("QuickSort Execution - Original");

    PROFILER_EXIT("QuickSort Test - Original");
}

void Test4_Optimization1() {
    PROFILER_ENTER("QuickSort Test - Optimization 1");

    std::vector<int> data(SORT_TEST_NUM_ENTRIES);
    PROFILER_ENTER("Data Generation - Optimization 1");
    for (int& num : data) {
        num = rand() % SORT_TEST_NUM_ENTRIES;
    }
    PROFILER_EXIT("Data Generation - Optimization 1");

    // Optimization 1: Use std::sort instead of custom QuickSort   
    PROFILER_ENTER("std::sort Execution - Optimization 1");
    std::sort(data.begin(), data.end());
    PROFILER_EXIT("std::sort Execution - Optimization 1");

    PROFILER_EXIT("QuickSort Test - Optimization 1");
}

void Test4_Optimization2() {
    PROFILER_ENTER("QuickSort Test - Optimization 2");

    std::vector<int> data(SORT_TEST_NUM_ENTRIES);
    PROFILER_ENTER("Data Generation - Optimization 2");
    for (int& num : data) {
        num = rand() % SORT_TEST_NUM_ENTRIES;
    }
    PROFILER_EXIT("Data Generation - Optimization 2");

    // Optimization 2: Use a more efficient pivot selection strategy for QuickSort
    PROFILER_ENTER("QuickSort with Median Pivot Execution");
    auto medianPivotQuickSort = [](std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int mid = low + (high - low) / 2;
            int pivot = arr[mid];  // Median pivot selection
            std::swap(arr[mid], arr[high]);
            int i = low - 1;
            for (int j = low; j < high; j++) {
                if (arr[j] <= pivot) {
                    i++;
                    std::swap(arr[i], arr[j]);
                }
            }
            std::swap(arr[i + 1], arr[high]);
            int pi = i + 1;

            QuickSort(arr, low, pi - 1);
            QuickSort(arr, pi + 1, high);
        }
    };
    medianPivotQuickSort(data, 0, SORT_TEST_NUM_ENTRIES - 1);
    PROFILER_EXIT("QuickSort with Median Pivot Execution");

    PROFILER_EXIT("QuickSort Test - Optimization 2");
}

void Test4_Optimization3() {
    PROFILER_ENTER("QuickSort Test - Optimization 3");

    std::vector<int> data(SORT_TEST_NUM_ENTRIES);
    PROFILER_ENTER("Data Generation - Optimization 3");
    for (int& num : data) {
        num = rand() % SORT_TEST_NUM_ENTRIES;
    }
    PROFILER_EXIT("Data Generation - Optimization 3");

    // Optimization 3: Use a hybrid sorting algorithm (QuickSort followed by std::sort for small arrays)
    PROFILER_ENTER("Hybrid Sort Execution");
    auto hybridQuickSort = [](std::vector<int>& arr, int low, int high) {
        const int threshold = 16;  // Threshold to switch to std::sort
        if (high - low > threshold) {
            int pivot = arr[high];
            int i = low - 1;
            for (int j = low; j < high; j++) {
                if (arr[j] <= pivot) {
                    i++;
                    std::swap(arr[i], arr[j]);
                }
            }
            std::swap(arr[i + 1], arr[high]);
            int pi = i + 1;

            QuickSort(arr, low, pi - 1);
            QuickSort(arr, pi + 1, high);
        } else {
            std::sort(arr.begin() + low, arr.begin() + high + 1);
        }
    };
    hybridQuickSort(data, 0, SORT_TEST_NUM_ENTRIES - 1);
    PROFILER_EXIT("Hybrid Sort Execution");

    PROFILER_EXIT("QuickSort Test - Optimization 3");
}

void Test4() {
    PROFILER_ENTER("Test 4 - Original");
    for (int i = 0; i < 100; ++i) {
        Test4_Original();
    }
    PROFILER_EXIT("Test 4 - Original");

    PROFILER_ENTER("Test 4 - Optimization 1");
    for (int i = 0; i < 100; ++i) {
        Test4_Optimization1();
    }
    PROFILER_EXIT("Test 4 - Optimization 1");
    
    PROFILER_ENTER("Test 4 - Optimization 2");
    for (int i = 0; i < 100; ++i) {
        Test4_Optimization2();
    }
    PROFILER_EXIT("Test 4 - Optimization 2");

    PROFILER_ENTER("Test 4 - Optimization 3");
    for (int i = 0; i < 100; ++i) {
        Test4_Optimization3();
    }
    PROFILER_EXIT("Test 4 - Optimization 3");
}

























void RunTest() {
    // Test1();
    Test2();
    Test3();
}

int main(int argc, char** argv) {
    profiler = Profiler::GetInstance();

    RunTest();

    // Calculate the statistics
    profiler->calculateStats();

    // Print out the statistics
    profiler->printStats();

    // Get statistics for the `Test 2 Speed Test` test
    ProfilerStats stats = profiler->calculateStats("Test 2 Speed Test");

    // Save to CSV
    profiler->saveStatsToCSV("profiler.csv");

    // Save to JSON
    profiler->saveStatsToJSON("profiler.json");

    // Reset the statistics
    profiler->reset();

    // Run test 4
    Test4();

    // Calculate and print stats
    profiler->calculateStats();
    profiler->printStats();

    // Save to CSV and JSON
    profiler->saveStatsToCSV("profiler_test4.csv");
    profiler->saveStatsToJSON("profiler_test4.json");

    delete profiler;
    profiler = nullptr;
    return 0;
}