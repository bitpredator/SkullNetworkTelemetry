#include "memory_scanner.h"

bool MemoryScanner::attach(const std::wstring& procName) {
    // TODO: implementazione reale
    return false;
}

uintptr_t MemoryScanner::find_pattern(const std::vector<uint8_t>& pattern,
                                      const std::string& mask)
{
    // TODO: implementazione reale
    return 0;
}

bool MemoryScanner::read_bytes(uintptr_t addr, void* buf, size_t size) const {
    return ReadProcessMemory(hProc, (LPCVOID)addr, buf, size, nullptr);
}
