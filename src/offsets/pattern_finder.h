#pragma once
#include <cstdint>
#include <vector>
#include <string>

class PatternFinder {
public:
    PatternFinder() = default;
    // find in module memory, return address or 0
    uintptr_t find(const std::string& pattern, const std::string& mask) const;

    // helper: convert signature like "48 8B ?? ??" to bytes/mask
    static void parse_pattern(const std::string& sig, std::vector<uint8_t>& outBytes, std::string& outMask);
};
