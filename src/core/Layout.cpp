#include "Layout.h"
#include <algorithm>

Rectangle computeAttitudeIndicatorRegion(int windowWidth, int windowHeight) {
    double size = std::min(windowWidth, windowHeight) * 0.6;
    double x = (windowWidth - size) / 2.0;
    double y = (windowHeight - size) / 2.0;

    Rectangle region;
    region.x = x;
    region.y = y;
    region.width = size;
    region.height = size;
    return region;
}