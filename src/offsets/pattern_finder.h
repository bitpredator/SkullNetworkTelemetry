#pragma once
#include <vector>
#include <string>
#include <cstdint>

class PatternFinder {
public:

    // Singleton accessor
    static PatternFinder& instance() {
        static PatternFinder inst;
        return inst;
    }

    // Cerca una pattern string "AA BB ?? CC"
    uintptr_t find(const std::string& pattern);

    // Converte la pattern in byte + mask
    static bool parse_pattern(const std::string& pattern,
                              std::vector<uint8_t>& bytes,
                              std::string& mask);

private:
    PatternFinder() = default;                    // costruttore privato
    PatternFinder(const PatternFinder&) = delete;
    PatternFinder& operator=(const PatternFinder&) = delete;
};
