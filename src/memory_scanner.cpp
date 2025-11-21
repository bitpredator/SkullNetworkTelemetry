#include "memory_scanner.h"
#include < Psapi.h >
#pragma comment(lib, "Psapi.lib")

MemoryScanner::MemoryScanner() {}

bool MemoryScanner::initialize() {
    HMODULE module = GetModuleHandleA(NULL);
    if (!module)
        return false;

    base_address = reinterpret_cast<uintptr_t>(module);

    MODULEINFO info;
    if (!GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(info)))
        return false;

    module_size = static_cast<size_t>(info.SizeOfImage);
    return true;
}

bool MemoryScanner::read(uintptr_t address, void* buffer, size_t size) const {
    SIZE_T bytesRead = 0;
    return ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(address),
                             buffer, size, &bytesRead) && bytesRead == size;
}

// ⭐ NUOVA FUNZIONE CHE RISOLVE L'ERRORE ⭐
uintptr_t MemoryScanner::find_pattern_bytes(const std::vector<int>& pattern) const {
    if (base_address == 0 || module_size == 0 || pattern.empty())
        return 0;

    const size_t pattern_len = pattern.size();
    const uintptr_t end = base_address + module_size - pattern_len;

    for (uintptr_t addr = base_address; addr < end; addr++) {

        bool found = true;

        for (size_t i = 0; i < pattern_len; i++) {
            int byte = pattern[i];

            // -1 = wildcard (??)
            if (byte != -1 && byte != *(unsigned char*)(addr + i)) {
                found = false;
                break;
            }
        }

        if (found)
            return addr;
    }

    return 0;
}
