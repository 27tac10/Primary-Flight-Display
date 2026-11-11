#ifndef TEXTPFD_H
#define TEXTPFD_H

#include "Aircraft.h"

// Text-based Primary Flight Display: reads data from an Aircraft and prints it.
// It holds no flight data of its own.
class TextPFD {
    public:
        explicit TextPFD(const Aircraft& aircraft) : aircraft(aircraft) {}

        void display() const;

    private:
        const Aircraft& aircraft;
};

#endif // TEXTPFD_H
