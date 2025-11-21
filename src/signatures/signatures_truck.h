#pragma once
#include <string>

namespace Signatures {

    //
    //  TRUCK INFO — Stable AOBs for ETS2 1.57+
    //

    // Dashboard → speed + rpm block
    static const std::string SIG_TRUCK_DASHBOARD =
        "F3 0F 10 89 ?? ?? ?? ?? F3 0F 10 81 ?? ?? ?? ??";

    // Lights bitmask (indicators, high beams, brake light)
    static const std::string SIG_TRUCK_LIGHTS =
        "0F B6 81 ?? ?? ?? ?? A8 ??";

    // Engine ON/OFF state
    static const std::string SIG_ENGINE_STATE =
        "0F B6 81 ?? ?? ?? ?? 83 F8 00";

    //
    // Extra (opzionali, potremo usarli dopo):
    //

    // Player position block (XYZ floats, stable in 1.35 → 1.57+)
    static const std::string SIG_PLAYER_POSITION =
        "F3 0F 10 ?? ?? ?? ?? ?? "
        "F3 0F 10 ?? ?? ?? ?? ?? "
        "F3 0F 10 ?? ?? ?? ?? ??";

    // Heading (radians)
    static const std::string SIG_PLAYER_HEADING =
        "F3 0F 10 40 ?? F3 0F 10 48 ??";

    // Speed (alternative)
    static const std::string SIG_VEHICLE_SPEED =
        "F3 0F 10 89 ?? ?? ?? ?? F3 0F 59 05 ?? ?? ?? ??";

}
