#include <iostream>

#include "Aircraft.h"
#include "TextPFD.h"

int main() {
    Aircraft aircraft;

    // Hard-coded flight data for now
    aircraft.setFlightNumber("NH123");
    aircraft.setAirspeed(250.0);
    aircraft.setGroundSpeed(265.0);
    aircraft.setVerticalSpeed(500.0);
    aircraft.setAltitude(10000.0);
    aircraft.setPitch(2.5);
    aircraft.setBank(-10.0);
    aircraft.setHeading(90.0);
    aircraft.setSlip(0.0);

    TextPFD textPfd(aircraft);
    textPfd.display();

    // Change data through the Aircraft; the TextPFD should show the new values.
    aircraft.setAltitude(10500.0);
    aircraft.setHeading(-5.0);  // wraps to 355
    aircraft.setSlip(0.3);

    std::cout << "\n(after update)\n";
    textPfd.display();

    return 0;
}
