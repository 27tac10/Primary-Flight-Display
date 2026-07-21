#ifndef SIMULATED_DATA_SOURCE_H
#define SIMULATED_DATA_SOURCE_H

#include "IDataSource.h"

class SimulatedDataSource : public IDataSource {
    public:
        FlightState update(double dt) override;
    private:
        double elapsedTime = 0.0; // Total elapsed time in seconds
};

#endif // SIMULATED_DATA_SOURCE_H