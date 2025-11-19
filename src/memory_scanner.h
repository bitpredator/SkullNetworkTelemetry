#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <cstdint>

class MemoryScanner {
public:
    MemoryScanner();
    bool initialize();

    // Cerca un pattern in memoria
    uintptr_t find_pattern(const char* pattern, const char* mask) const;

    // Legge memoria (del nostro stesso processo)
    bool read(uintptr_t address, void* buffer, size_t size) const;

private:
    uintptr_t base_address = 0;
    size_t module_size = 0;

    bool get_module_info();
};
