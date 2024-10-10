#include "profiler.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <math.h>

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
    ;
}

void RunTest() {
    Test1();
    Test2();
}

int main(int argc, char** argv) {
    profiler = new Profiler();

    RunTest();

    delete profiler;
    profiler = nullptr;
    return 0;
}