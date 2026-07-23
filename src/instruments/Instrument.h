#ifndef H_INSTRUMENT
#define H_INSTRUMENT

#include "../render/DrawCommand.h"
#include "../core/FlightState.h"

class Instrument {
    public:
        virtual ~Instrument() = default;
        virtual DrawList render(const FlightState& state, const Rectangle& region) const = 0;
};

#endif // H_INSTRUMENT