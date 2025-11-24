#include "offsets.h"
#include "pattern_finder.h"
#include "signatures.h"
#include "memory_scanner.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

GameOffsets g_offsets;

bool find_all_offsets() {
    PatternFinder& finder = PatternFinder::instance();

    json sigs;
    bool loaded = false;

    // Player position
    {
        uintptr_t addr = 0;
        if (loaded && sigs.contains("player_position")) {
            std::string pat = sigs["player_position"]["pattern"].get<std::string>();
            addr = finder.find(pat);
        }

        if (!addr) {
            std::vector<uint8_t> bytes;
            std::string mask;
            PatternFinder::parse_pattern(FALLBACK_SIG_PLAYER_POS, bytes, mask);

            addr = MemoryScanner::instance().find_pattern(bytes, mask);
        }

        g_offsets.player_position = addr;
    }

    return (g_offsets.player_position &&
            g_offsets.vehicle_speed &&
            g_offsets.vehicle_heading);
}
