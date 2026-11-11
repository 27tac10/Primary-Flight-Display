#include <cmath>
#include <format>
#include <iostream>
#include <string>

#include "PFD.h"

namespace {

std::string slipDescription(double slip) {
    if (std::abs(slip) < 0.05) {
        return "coordinated";
    }
    return slip < 0.0 ? "slip L" : "skid R";
}

} // namespace

void PFD::display() const {
    const FlightData& d = aircraft.getData();

    // Round heading first so 359.6 shows as 000, not 360.
    int heading = static_cast<int>(std::lround(d.heading)) % 360;

    std::cout << "========== PFD ==========\n"
              << std::format("Flight        : {}\n", d.flightNumber)
              << std::format("Attitude      : pitch {:+.1f} deg   bank {:+.1f} deg\n", d.pitch, d.bank)
              << std::format("Airspeed      : {:.0f} kt\n", d.airspeed)
              << std::format("Altitude      : {:.0f} ft\n", d.altitude)
              << std::format("Vertical speed: {:+.0f} ft/min\n", d.verticalSpeed)
              << std::format("Heading       : {:03d} deg\n", heading)
              << std::format("Turn/slip     : {:+.1f} ({})\n", d.slip, slipDescription(d.slip))
              << std::format("Ground speed  : {:.0f} kt\n", d.groundSpeed)
              << "=========================\n";
}
