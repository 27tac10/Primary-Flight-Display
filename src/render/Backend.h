#ifndef BACKEND_H
#define BACKEND_H

#include "DrawCommand.h"

class Backend {
    public:
        virtual ~Backend() = default;
        virtual void render(const DrawList& commands) = 0;
};

#endif // BACKEND_H