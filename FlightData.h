#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <string>

// Snapshot of the aircraft's current flight state.
struct FlightData {
    std::string flightNumber = "";
    double airspeed      = 0.0;  // indicated airspeed, kt
    double groundSpeed   = 0.0;  // kt
    double verticalSpeed = 0.0;  // ft/min, positive = climbing
    double altitude      = 0.0;  // ft
    double pitch         = 0.0;  // deg, positive = nose up
    double bank          = 0.0;  // deg, positive = right wing down
    double heading       = 0.0;  // deg magnetic, 0-360
    double slip          = 0.0;  // 0 = coordinated, negative = slip left, positive = skid right
};

#endif // FLIGHTDATA_H
