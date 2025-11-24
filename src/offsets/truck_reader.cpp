#include "truck_reader.h"
#include <iostream>
#include <vector>

TruckReader::TruckReader(MemoryScanner* scanner_, OffsetsResolver* offsets_)
    : scanner(scanner_), offsets(offsets_) {}

template<typename T>
bool TruckReader::read_val(uintptr_t addr, T& out) const {
    if (!addr) return false;
    return scanner->read_bytes(addr, &out, sizeof(T));  // CORRETTO
}

std::string TruckReader::read_string_ptr(uintptr_t ptr, size_t maxLen) const {
    if (!ptr) return "";

    uintptr_t strPtr = 0;

    // Prima prova: ptr -> char*
    if (scanner->read_bytes(ptr, &strPtr, sizeof(strPtr)) && strPtr) {

        std::vector<char> buf(maxLen);
        if (scanner->read_bytes(strPtr, buf.data(), maxLen)) {
            buf[maxLen - 1] = '\0';
            return std::string(buf.data());
        }
    }

    // fallback: ptr è già indirizzo della stringa
    std::vector<char> buf(maxLen);
    if (scanner->read_bytes(ptr, buf.data(), maxLen)) {
        buf[maxLen - 1] = '\0';
        return std::string(buf.data());
    }

    return "";
}

bool TruckReader::read(TruckInfo& out) {
    if (!offsets) return false;

    uintptr_t base = offsets->truck.dashboardPtr;
    if (!base) {
        std::cerr << "[TruckReader] dashboardPtr == 0\n";
        return false;
    }

    // Esempio di struttura (temporanea)
    float speed_ms = 0.0f;
    if (read_val(base + 0x10, speed_ms))
        out.speed_kph = speed_ms * 3.6f;

    read_val(base + 0x14, out.rpm);
    read_val(base + 0x18, out.gear);
    read_val(base + 0x1C, out.fuel);
    read_val(base + 0x20, out.damage);

    out.make  = read_string_ptr(base + 0x30, 64);
    out.model = read_string_ptr(base + 0x38, 64);

    return true;
}
