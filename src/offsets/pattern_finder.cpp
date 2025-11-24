#include "pattern_finder.h"
#include "memory_scanner.h"

#include <sstream>
#include <iostream>


// ---------------------------------------------------------------------
// CONVERSIONE PATTERN in BYTES + MASK
// ---------------------------------------------------------------------
bool PatternFinder::parse_pattern(const std::string& pattern,
                                  std::vector<uint8_t>& bytes,
                                  std::string& mask)
{
    std::istringstream iss(pattern);
    std::string token;

    bytes.clear();
    mask.clear();

    while (iss >> token) {
        if (token == "??" || token == "?") {
            bytes.push_back(0x00);
            mask += "?";
        } else {
            uint8_t byte = static_cast<uint8_t>(std::stoul(token, nullptr, 16));
            bytes.push_back(byte);
            mask += "x";
        }
    }
    return true;
}


// ---------------------------------------------------------------------
// TROVA UNA SIGNATURE
// ---------------------------------------------------------------------
uintptr_t PatternFinder::find(const std::string& pattern)
{
    std::vector<uint8_t> bytes;
    std::string mask;

    if (!parse_pattern(pattern, bytes, mask))
        return 0;

    // Usa il singleton MemoryScanner
    return MemoryScanner::instance().find_pattern(bytes, mask);
}
