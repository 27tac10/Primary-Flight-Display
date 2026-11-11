#ifndef PFD_H
#define PFD_H

#include "Aircraft.h"

// Primary Flight Display: reads data from an Aircraft and displays it.
// It holds no flight data of its own.
class PFD {
    public:
        explicit PFD(const Aircraft& aircraft) : aircraft(aircraft) {}

        void display() const;

    private:
        const Aircraft& aircraft;
};

#endif // PFD_H
