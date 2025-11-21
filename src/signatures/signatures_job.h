#pragma once
#include <string>

namespace Signatures {

    //
    //  JOB / CARGO INFO — Stable AOBs for ETS2 1.57+
    //

    // Active job pointer (Freight manager → job struct)
    static const std::string SIG_JOB_DATA =
        "48 8B 0D ?? ?? ?? ?? 48 8B 81 ?? ?? ?? ??";

    // Trailer data pointer (cargo mass, wear, hookups, etc.)
    static const std::string SIG_TRAILER_DATA =
        "48 8B 81 ?? ?? ?? ?? 48 8B 90 ?? ?? ?? ??";

}
