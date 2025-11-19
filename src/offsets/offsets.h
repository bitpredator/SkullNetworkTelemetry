#pragma once
#include <cstdint>

struct GameOffsets {
    uintptr_t player_position = 0;
    uintptr_t vehicle_speed   = 0;
    uintptr_t vehicle_heading = 0;
    uintptr_t truck_name_ptr  = 0;
};

extern GameOffsets g_offsets;

// call this at startup (returns true if at least one signature matched)
bool find_all_offsets();
