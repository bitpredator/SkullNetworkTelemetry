#pragma once
#include <vector>
#include <string>
#include <windows.h>

class MemoryScanner {
public:
    // Singleton accessor
    static MemoryScanner& instance() {
        static MemoryScanner inst;
        return inst;
    }

    bool attach(const std::wstring& procName);
    uintptr_t find_pattern(const std::vector<uint8_t>& pattern, const std::string& mask);
    bool read_bytes(uintptr_t address, void* buffer, size_t size) const;

private:
    MemoryScanner() = default;            // Singleton: costruttore privato
    MemoryScanner(const MemoryScanner&) = delete;
    MemoryScanner& operator=(const MemoryScanner&) = delete;

private:
    HANDLE hProc = NULL;
};
