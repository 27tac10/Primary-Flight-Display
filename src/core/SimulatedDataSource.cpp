#include "SimulatedDataSource.h"
#include <algorithm>
#include <cmath>

FlightState SimulatedDataSource::update(double dt) {
    elapsedTime += dt;

    FlightState state;
    state.pitch = 10.0 * sin(elapsedTime); // Simulate pitch oscillation
    state.roll = 5.0 * cos(elapsedTime);   // Simulate roll oscillation
    state.airspeed = 250.0 + 10.0 * sin(elapsedTime / 2.0); // Simulate airspeed variation
    state.altitude = 10000.0 + 500.0 * sin(elapsedTime / 3.0); // Simulate altitude variation
    state.heading = fmod(90.0 + elapsedTime * 10.0, 360.0); // Simulate heading change
    state.verticalSpeed = 500.0 * sin(elapsedTime / 4.0); // Simulate vertical speed variation

    state.pitch = std::clamp(state.pitch, -90.0, 90.0);
    state.roll = std::clamp(state.roll, -180.0, 180.0);
    state.airspeed = std::max(0.0, state.airspeed);
    state.altitude = std::max(0.0, state.altitude);

    return state;
}