#pragma once
#include <string>
#include "telemetry_payload.h"
#include "memory_scanner.h"
#include "offsets_resolver.h"

struct TruckInfo {
    std::string make;
    std::string model;
    float speed_kph = 0.0f;
    float rpm = 0.0f;
    int gear = 0;
    float fuel = 0.0f;
    float damage = 0.0f;
};

class TruckReader {
public:
    TruckReader(MemoryScanner* scanner, OffsetsResolver* offsets);
    bool read(TruckInfo& out);

private:
    MemoryScanner* scanner;
    OffsetsResolver* offsets;

    // helper
    std::string read_string_ptr(uintptr_t ptr, size_t maxLen = 128) const;
    template<typename T> bool read_val(uintptr_t addr, T& out) const;
};
