#include <cmath>

#include "Aircraft.h"

// Keep heading in the range [0, 360).
void Aircraft::setHeading(double heading) {
    heading = std::fmod(heading, 360.0);
    if (heading < 0.0) {
        heading += 360.0;
    }
    data.heading = heading;
}
