#pragma once
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <string>

class MemoryScanner {
public:
    MemoryScanner();

    bool initialize();

    // Nuovo metodo necessario al PatternFinder
    uintptr_t find_pattern_bytes(const std::vector<int>& pattern) const;

    bool read(uintptr_t address, void* buffer, size_t size) const;

private:
    uintptr_t base_address = 0;
    size_t module_size = 0;
};
