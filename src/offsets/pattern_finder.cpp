#include "pattern_finder.h"
#include "memory_scanner.h"
#include <sstream>

extern MemoryScanner g_scanner;

void PatternFinder::parse_pattern(const std::string& patternStr,
                                  std::vector<int>& outPattern) const
{
    outPattern.clear();
    std::istringstream iss(patternStr);
    std::string token;

    while (iss >> token)
    {
        if (token == "??" || token == "?")
        {
            outPattern.push_back(-1); // wildcard
        }
        else
        {
            int value = std::stoi(token, nullptr, 16);
            outPattern.push_back(value & 0xFF);
        }
    }
}

uintptr_t PatternFinder::find(const std::string& pattern) const
{
    std::vector<int> parsed;
    parse_pattern(pattern, parsed);

    if (parsed.empty())
        return 0;

    return g_scanner.find_pattern_bytes(parsed);
}
