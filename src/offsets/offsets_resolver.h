#pragma once
#include <cstdint>

struct TruckOffsets {
    uintptr_t dashboardPtr = 0;
    uintptr_t lightsPtr = 0;
    uintptr_t engineStatePtr = 0;
};

struct JobOffsets {
    uintptr_t jobDataPtr = 0;
    uintptr_t trailerDataPtr = 0;
};

class OffsetsResolver {
public:
    bool resolve_truck();
    bool resolve_job();
    bool resolve_all();

    TruckOffsets truck;
    JobOffsets job;
};

extern OffsetsResolver g_offsets;
