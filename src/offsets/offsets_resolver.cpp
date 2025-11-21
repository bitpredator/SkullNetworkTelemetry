#include "offsets_resolver.h"
#include "pattern_finder.h"
#include "signatures_truck.h"
#include "signatures_job.h"
#include "memory_scanner.h"

#include <iostream>
#include <cstdint>

// Istanza globale dello scanner utilizzata da PatternFinder internamente
MemoryScanner g_scanner;

// Istanza globale del PatternFinder
PatternFinder g_finder;

// Definizione dell'istanza globale dichiarata in offsets_resolver.h
OffsetsResolver g_offsets;

// Helper: resolve RIP-relative addressing (typical: 48 8B 05 xx xx xx xx -> imm32 at offset 3)
static uintptr_t resolve_rip_relative(uintptr_t instr_addr, int imm32_offset = 3) {
    if (instr_addr == 0) return 0;

    int32_t imm = 0;
    // leggiamo l'imm32 che si trova a instr_addr + imm32_offset
    if (!g_scanner.read(instr_addr + imm32_offset, &imm, sizeof(imm))) {
        return 0;
    }

    // next_instruction = instr_addr + imm32_offset + sizeof(int32)
    uintptr_t next_instruction = instr_addr + imm32_offset + sizeof(int32_t);
    return next_instruction + static_cast<int64_t>(imm);
}

// Internal small helper: try resolve pattern and RIP-relative pointer if applicable
static uintptr_t find_and_resolve(const std::string& pattern) {
    uintptr_t instr = g_finder.find(pattern);
    if (!instr) return 0;

    // Try to detect common opcodes that contain RIP-relative imm32:
    // - sequences like "48 8B 05 xx xx xx xx" (mov rax, [rip+imm32])
    // - "48 8B 0D xx xx xx xx", "48 8B 1D xx xx xx xx"
    // We'll examine the first bytes at instr to determine if we should try resolve_rip_relative.
    unsigned char firstBytes[4] = {0};
    if (!g_scanner.read(instr, firstBytes, sizeof(firstBytes))) {
        // can't read instruction bytes -> return instr (debuggable)
        return instr;
    }

    // check for 48 8B 05 / 48 8B 0D / 48 8B 1D
    if (firstBytes[0] == 0x48 && firstBytes[1] == 0x8B &&
        (firstBytes[2] == 0x05 || firstBytes[2] == 0x0D || firstBytes[2] == 0x1D)) {
        uintptr_t resolved = resolve_rip_relative(instr, 3);
        if (resolved) return resolved;
    }

    // other patterns could use different encodings; you can add cases here.
    // If we couldn't resolve, return the instruction address for debug fallback.
    return instr;
}

//
// RISOLVE TRUCK INFO (speed, rpm, lights, engine state, ecc.)
//
bool OffsetsResolver::resolve_truck() {
    std::cout << "[Offsets] Resolving Truck Offsets..." << std::endl;

    // Dashboard: spesso pattern like "48 8B 05 ?? ?? ?? ?? ..." -> rip-relative pointer to struct
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_TRUCK_DASHBOARD);
        if (!resolved) {
            std::cout << "[Offsets] Truck Dashboard: NOT FOUND!" << std::endl;
        } else {
            truck.dashboardPtr = resolved;
            std::cout << "[Offsets] Truck Dashboard -> 0x" << std::hex << truck.dashboardPtr << std::dec << std::endl;
        }
    }

    // Lights
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_TRUCK_LIGHTS);
        if (!resolved) {
            std::cout << "[Offsets] Truck Lights: NOT FOUND!" << std::endl;
        } else {
            truck.lightsPtr = resolved;
            std::cout << "[Offsets] Truck Lights -> 0x" << std::hex << truck.lightsPtr << std::dec << std::endl;
        }
    }

    // Engine state
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_ENGINE_STATE);
        if (!resolved) {
            std::cout << "[Offsets] Truck EngineState: NOT FOUND!" << std::endl;
        } else {
            truck.engineStatePtr = resolved;
            std::cout << "[Offsets] Truck EngineState -> 0x" << std::hex << truck.engineStatePtr << std::dec << std::endl;
        }
    }

    // Optionally, you can add more resolutions: damage, wheels, etc.

    return (truck.dashboardPtr != 0) && (truck.lightsPtr != 0) && (truck.engineStatePtr != 0);
}

//
// RISOLVE JOB INFO (cargo, mass, route, delivery time, payment, ecc.)
//
bool OffsetsResolver::resolve_job() {
    std::cout << "[Offsets] Resolving Job Offsets..." << std::endl;

    // Job data
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_JOB_DATA);
        if (!resolved) {
            std::cout << "[Offsets] Job Data: NOT FOUND!" << std::endl;
        } else {
            job.jobDataPtr = resolved;
            std::cout << "[Offsets] Job Data -> 0x" << std::hex << job.jobDataPtr << std::dec << std::endl;
        }
    }

    // Trailer data
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_TRAILER_DATA);
        if (!resolved) {
            std::cout << "[Offsets] Trailer Data: NOT FOUND!" << std::endl;
        } else {
            job.trailerDataPtr = resolved;
            std::cout << "[Offsets] Trailer Data -> 0x" << std::hex << job.trailerDataPtr << std::dec << std::endl;
        }
    }

    return (job.jobDataPtr != 0) && (job.trailerDataPtr != 0);
}

//
// RISOLVE TUTTO INSIEME (truck + job)
//
bool OffsetsResolver::resolve_all() {
    std::cout << "\n[Offsets] Starting full offset resolution...\n";

    // Initialize memory scanner (must be done before any pattern-finds)
    if (!g_scanner.initialize()) {
        std::cout << "[Offsets] ERROR: MemoryScanner initialization failed!" << std::endl;
        return false;
    }

    bool okTruck = resolve_truck();
    bool okJob   = resolve_job();

    if (!okTruck) {
        std::cout << "[Offsets] WARNING: Some truck offsets NOT resolved. Check signatures or run in debug mode." << std::endl;
    }
    if (!okJob) {
        std::cout << "[Offsets] WARNING: Some job offsets NOT resolved. Check signatures or run in debug mode." << std::endl;
    }

    return okTruck && okJob;
}
