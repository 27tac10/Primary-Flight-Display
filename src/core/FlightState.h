#ifndef FLIGHT_STATE_H
#define FLIGHT_STATE_H

struct FlightState {
    double pitch = 0.0;         // Pitch angle in degrees
    double roll = 0.0;          // Roll angle in degrees
    double airspeed = 0.0;      // Airspeed in knots
    double altitude = 0.0;      // Altitude in feet
    double heading = 0.0;       // Heading in degrees
    double verticalSpeed = 0.0; // Vertical speed in feet per minute
};

#endif // FLIGHT_STATE_H