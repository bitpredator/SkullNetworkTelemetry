#include "truck_reader.h"
#include <iostream>
#include <vector>

TruckReader::TruckReader(MemoryScanner* scanner_, OffsetsResolver* offsets_)
    : scanner(scanner_), offsets(offsets_) {}

template<typename T>
bool TruckReader::read_val(uintptr_t addr, T& out) const {
    if (!addr) return false;
    return scanner->read(addr, &out, sizeof(T));
}

std::string TruckReader::read_string_ptr(uintptr_t ptr, size_t maxLen) const {
    if (!ptr) return "";
    // first read pointer (RIP relative resolved by offsets resolver usually)
    // some signatures will point directly to struct; others to pointer-to-char
    // try read as pointer to char*
    uintptr_t strPtr = 0;
    if (scanner->read(ptr, &strPtr, sizeof(strPtr)) && strPtr) {
        // read string bytes
        std::vector<char> buf;
        buf.resize(maxLen);
        if (scanner->read(strPtr, buf.data(), maxLen)) {
            // ensure null-termination
            buf[maxLen-1] = '\0';
            return std::string(buf.data());
        }
    } else {
        // maybe the signature already points to chars
        std::vector<char> buf;
        buf.resize(maxLen);
        if (scanner->read(ptr, buf.data(), maxLen)) {
            buf[maxLen-1] = '\0';
            return std::string(buf.data());
        }
    }
    return "";
}

bool TruckReader::read(TruckInfo& out) {
    if (!offsets) return false;

    // Example: offsets.truck.dashboardPtr is an instruction address,
    // usually you need to compute actual data pointer (RIP+imm32) — offsets_resolver should provide resolved data pointer.
    // Here we assume offsets.truck.dashboardPtr points to a struct or pointer; adjust according to your resolved addresses.

    uintptr_t base = offsets->truck.dashboardPtr;
    if (!base) {
        std::cerr << "[TruckReader] dashboardPtr == 0\n";
        return false;
    }

    // Example layout (MUST BE ADJUSTED):  
    // base + 0x10 -> float speed (m/s)  
    // base + 0x14 -> float rpm  
    // base + 0x18 -> int gear  
    // base + 0x1C -> float fuel fraction  
    // base + 0x20 -> float damage fraction  
    // base + 0x30 -> pointer to make/model strings (split or two pointers)

    float speed_ms = 0.0f;
    if (read_val<float>(base + 0x10, speed_ms)) out.speed_kph = speed_ms * 3.6f;

    float rpm = 0.0f;
    read_val<float>(base + 0x14, rpm);
    out.rpm = rpm;

    int gear = 0;
    read_val<int>(base + 0x18, gear);
    out.gear = gear;

    float fuel = 0.0f;
    read_val<float>(base + 0x1C, fuel);
    out.fuel = fuel;

    float damage = 0.0f;
    read_val<float>(base + 0x20, damage);
    out.damage = damage;

    // read make/model pointers (example offsets)
    out.make = read_string_ptr(base + 0x30, 64);
    out.model = read_string_ptr(base + 0x38, 64);

    // Some fields may be reversed or at different offsets; log if missing
    return true;
}
