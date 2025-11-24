#include "offsets_resolver.h"
#include "pattern_finder.h"
#include "signatures_truck.h"
#include "signatures_job.h"
#include "memory_scanner.h"

#include <iostream>
#include <cstdint>

// NON CREARE istanze, usa solo i singleton
// MemoryScanner g_scanner;          ❌ SBAGLIATO
// PatternFinder g_finder;           ❌ SBAGLIATO

// Istanza globale dell’offset resolver (questa è ok)
OffsetsResolver g_offsets;


// Helper: resolve RIP-relative addressing
static uintptr_t resolve_rip_relative(uintptr_t instr_addr, int imm32_offset = 3) {
    if (instr_addr == 0) return 0;

    int32_t imm = 0;

    // Usa il singleton
    if (!MemoryScanner::instance().read_bytes(instr_addr + imm32_offset, &imm, sizeof(imm))) {
        return 0;
    }

    uintptr_t next_instruction = instr_addr + imm32_offset + sizeof(int32_t);
    return next_instruction + static_cast<int64_t>(imm);
}


// Helper di pattern find + RIP resolver
static uintptr_t find_and_resolve(const std::string& pattern) {

    uintptr_t instr = PatternFinder::instance().find(pattern);
    if (!instr) return 0;

    unsigned char firstBytes[4] = {0};

    if (!MemoryScanner::instance().read_bytes(instr, firstBytes, sizeof(firstBytes))) {
        return instr; // fallback: return instruction address
    }

    // Match di 48 8B 05/0D/1D xx xx xx xx
    if (firstBytes[0] == 0x48 &&
        firstBytes[1] == 0x8B &&
        (firstBytes[2] == 0x05 || firstBytes[2] == 0x0D || firstBytes[2] == 0x1D)) 
    {
        uintptr_t resolved = resolve_rip_relative(instr, 3);
        if (resolved) return resolved;
    }

    return instr;
}


// ------------------------------------------------------------
// TRUCK OFFSETS
// ------------------------------------------------------------
bool OffsetsResolver::resolve_truck() {
    std::cout << "[Offsets] Resolving Truck Offsets..." << std::endl;

    // Dashboard pointer
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_TRUCK_DASHBOARD);
        truck.dashboardPtr = resolved;

        if (!resolved)
            std::cout << "[Offsets] Truck Dashboard: NOT FOUND!\n";
        else
            std::cout << "[Offsets] Truck Dashboard -> 0x" 
                      << std::hex << resolved << std::dec << "\n";
    }

    // Lights
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_TRUCK_LIGHTS);
        truck.lightsPtr = resolved;

        if (!resolved)
            std::cout << "[Offsets] Truck Lights: NOT FOUND!\n";
        else
            std::cout << "[Offsets] Truck Lights -> 0x" 
                      << std::hex << resolved << std::dec << "\n";
    }

    // Engine state
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_ENGINE_STATE);
        truck.engineStatePtr = resolved;

        if (!resolved)
            std::cout << "[Offsets] Truck EngineState: NOT FOUND!\n";
        else
            std::cout << "[Offsets] EngineState -> 0x"
                      << std::hex << resolved << std::dec << "\n";
    }

    return truck.dashboardPtr && truck.lightsPtr && truck.engineStatePtr;
}


// ------------------------------------------------------------
// JOB OFFSETS
// ------------------------------------------------------------
bool OffsetsResolver::resolve_job() {
    std::cout << "[Offsets] Resolving Job Offsets..." << std::endl;

    // Job data
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_JOB_DATA);
        job.jobDataPtr = resolved;

        if (!resolved)
            std::cout << "[Offsets] Job Data: NOT FOUND!\n";
        else
            std::cout << "[Offsets] Job Data -> 0x"
                      << std::hex << resolved << std::dec << "\n";
    }

    // Trailer data
    {
        uintptr_t resolved = find_and_resolve(Signatures::SIG_TRAILER_DATA);
        job.trailerDataPtr = resolved;

        if (!resolved)
            std::cout << "[Offsets] Trailer Data: NOT FOUND!\n";
        else
            std::cout << "[Offsets] Trailer Data -> 0x"
                      << std::hex << resolved << std::dec << "\n";
    }

    return job.jobDataPtr && job.trailerDataPtr;
}


// ------------------------------------------------------------
// MAIN RESOLUTION
// ------------------------------------------------------------
bool OffsetsResolver::resolve_all() {
    std::cout << "\n[Offsets] Starting full offset resolution...\n";

    // ATTACH AL PROCESSO using singleton
    if (!MemoryScanner::instance().attach(L"eurotrucks2.exe")) {
        std::cout << "[Offsets] ERROR: Could not attach to eurotrucks2.exe!\n";
        return false;
    }

    bool okTruck = resolve_truck();
    bool okJob   = resolve_job();

    return okTruck && okJob;
}
