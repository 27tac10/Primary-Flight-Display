#ifndef IDATA_SOURCE_H
#define IDATA_SOURCE_H

#include "FlightState.h"

class IDataSource {

    public:
        virtual ~IDataSource() = default;
        virtual FlightState update(double dt) = 0;

};

#endif // IDATA_SOURCE_H