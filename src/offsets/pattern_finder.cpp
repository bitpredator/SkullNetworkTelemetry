#include "pattern_finder.h"
#include "memory_scanner.h"
#include <cstring>

extern MemoryScanner g_scanner; // ensure you have a global scanner instance

void PatternFinder::parse_pattern(const std::string& sig, std::vector<uint8_t>& outBytes, std::string& outMask) {
    outBytes.clear();
    outMask.clear();
    size_t i = 0;
    while (i < sig.size()) {
        if (sig[i] == ' ') { ++i; continue; }
        if (sig[i] == '?') {
            outBytes.push_back(0);
            outMask.push_back('?');
            // skip both ? or ?? tokens
            if (i + 1 < sig.size() && sig[i+1] == '?') i += 2; else ++i;
        } else {
            // read two hex
            auto hex = sig.substr(i, 2);
            uint8_t b = static_cast<uint8_t>(std::stoul(hex, nullptr, 16));
            outBytes.push_back(b);
            outMask.push_back('x');
            i += 2;
        }
    }
}

uintptr_t PatternFinder::find(const std::string& pattern, const std::string& mask) const {
    // pattern is already parsed form: better to use parse_pattern then call scanner.find_pattern_bytes
    std::vector<uint8_t> bytes;
    std::string computedMask;
    parse_pattern(pattern, bytes, computedMask);

    // Use MemoryScanner's find by bytes (implement below)
    return g_scanner.find_pattern_bytes(bytes.data(), computedMask.c_str(), bytes.size());
}
