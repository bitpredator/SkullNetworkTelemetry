#include "memory_scanner.h"
#include <psapi.h>
#include <iostream>

MemoryScanner::MemoryScanner() {}

bool MemoryScanner::initialize() {
    return get_module_info();
}

bool MemoryScanner::get_module_info() {
    HMODULE hModule = GetModuleHandle(nullptr);
    if (!hModule) return false;

    MODULEINFO modInfo = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo)))
        return false;

    base_address = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
    module_size = static_cast<size_t>(modInfo.SizeOfImage);

    std::cout << "[MemoryScanner] Base: 0x" << std::hex << base_address 
              << " Size: 0x" << module_size << std::dec << std::endl;
    return true;
}

bool MemoryScanner::read(uintptr_t address, void* buffer, size_t size) const {
    if (!address) return false;
    memcpy(buffer, reinterpret_cast<void*>(address), size);
    return true;
}

uintptr_t MemoryScanner::find_pattern(const char* pattern, const char* mask) const {
    size_t pattern_len = strlen(mask);

    uintptr_t start = base_address;
    uintptr_t end   = base_address + module_size - pattern_len;

    for (uintptr_t addr = start; addr < end; addr++) {
        bool found = true;

        for (size_t i = 0; i < pattern_len; i++) {
            if (mask[i] != '?' &&
                pattern[i] != *(char*)(addr + i)) {

                found = false;
                break;
            }
        }

        if (found) {
            return addr;
        }
    }

    return 0;
}
