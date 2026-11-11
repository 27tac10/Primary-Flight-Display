#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>

#include "FlightData.h"

// Core class: owns the flight data. All changes go through here.
class Aircraft {
    public:
        Aircraft() = default;

        // Read-only access for displays (e.g. PFD)
        const FlightData& getData() const { return data; }

        // Setters
        void setFlightNumber(const std::string& flightNumber) { data.flightNumber = flightNumber; }
        void setAirspeed(double airspeed) { data.airspeed = airspeed; }
        void setGroundSpeed(double groundSpeed) { data.groundSpeed = groundSpeed; }
        void setVerticalSpeed(double verticalSpeed) { data.verticalSpeed = verticalSpeed; }
        void setAltitude(double altitude) { data.altitude = altitude; }
        void setPitch(double pitch) { data.pitch = pitch; }
        void setBank(double bank) { data.bank = bank; }
        void setHeading(double heading);
        void setSlip(double slip) { data.slip = slip; }

    private:
        FlightData data;
};

#endif // AIRCRAFT_H
