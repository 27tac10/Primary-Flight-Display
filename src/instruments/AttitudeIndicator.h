#ifndef H_ATTITUDEINDICATOR 
#define H_ATTITUDEINDICATOR   

#include "Instrument.h"

class AttitudeIndicator : public Instrument {
    public: DrawList render(const FlightState& state, const Rectangle& region) const override;

};

#endif // H_ATTITUDEINDICATOR  