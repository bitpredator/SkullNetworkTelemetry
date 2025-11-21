#pragma once
#include <string>
#include <vector>

class PatternFinder {
public:
    // Cerca un pattern in formato sig: "F3 0F 10 ?? ?? 89"
    uintptr_t find(const std::string& pattern) const;

private:
    // Converte la stringa signature → vector<int> con wildcard -1
    void parse_pattern(const std::string& patternStr, std::vector<int>& outPattern) const;
};
