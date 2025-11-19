#include "offsets.h"
#include "pattern_finder.h"
#include "signatures.h"
#include "memory_scanner.h"

#include <fstream>
#include <nlohmann/json.hpp> // optional: if you avoid external libs, load JSON manually

using json = nlohmann::json;

GameOffsets g_offsets;

// attempt to load signatures.json from module dir; fallback to built-in signatures
static bool load_signatures_from_file(const std::string& path, json& out) {
    std::ifstream in(path);
    if (!in.is_open()) return false;
    try {
        in >> out;
        return true;
    } catch(...) {
        return false;
    }
}

bool find_all_offsets() {
    PatternFinder finder;

    // try loading JSON signatures (signatures.json located next to DLL)
    json sigs;
    bool loaded = load_signatures_from_file("signatures.json", sigs);

    // Player position
    {
        uintptr_t addr = 0;
        if (loaded && sigs.contains("player_position")) {
            std::string pat = sigs["player_position"]["pattern"].get<std::string>();
            addr = finder.find(pat, ""); // pattern parser will derive mask
        }
        if (!addr) {
            // fallback to built-in small signature
            std::vector<uint8_t> bytes;
            std::string mask;
            PatternFinder::parse_pattern(FALLBACK_SIG_PLAYER_POS, bytes, mask);
            addr = g_scanner.find_pattern_bytes(bytes.data(), mask.c_str(), bytes.size());
        }
        g_offsets.player_position = addr;
    }

    // other offsets: vehicle_speed, vehicle_heading, truck_name_ptr...
    // (repeat same logic; omitted for brevity)

    // success if at least position+speed+heading found
    return (g_offsets.player_position && g_offsets.vehicle_speed && g_offsets.vehicle_heading);
}
