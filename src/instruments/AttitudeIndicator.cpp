#include "AttitudeIndicator.h"
#include <cmath>

DrawList AttitudeIndicator::render(const FlightState& state, const Rectangle& region) const {
    Point center;
    center.x = region.x + region.width / 2.0;
    center.y = region.y + region.height / 2.0;

    double pixelsPerDegree = region.height / 40.0;
    double horizonY = center.y + state.pitch * pixelsPerDegree;

    double left = center.x - region.width;
    double right = center.x + region.width;
    double top = center.y - region.height;
    double bottom = center.y + region.height;

    double tickHalfWidth = region.width * 0.15;

    DrawList list;
    PushClipCommand clip;
    clip.clipRect = region;
    list.push_back(clip);

    PushTransformCommand rotate;
    rotate.pivot = center;
    rotate.rotationAngle = -state.roll;
    list.push_back(rotate);

    FilledPolygonCommand sky;
    sky.listOfPoints = {{left, top}, {right, top}, {right, horizonY}, {left, horizonY}};
    sky.fillColor = {0.3, 0.44, 0.9, 1.0};
    list.push_back(sky);

    FilledPolygonCommand ground;
    ground.listOfPoints = {{left, horizonY}, {right, horizonY}, {right, bottom}, {left, bottom}};
    ground.fillColor = {0.55, 0.35, 0.15, 1.0};
    list.push_back(ground);

    // --- Pitch ladder: EXACTLY as it was before, unchanged ---
    for (int p = -30; p <= 30; p += 10) {
        if (p == 0) {
            continue;
        }

        double tickY = center.y + (state.pitch - p) * pixelsPerDegree;

        LineCommand tick;
        tick.start = {center.x - tickHalfWidth, tickY};
        tick.end = {center.x + tickHalfWidth, tickY};
        tick.width = 2.0;
        tick.color = {1.0, 1.0, 1.0, 1.0};
        list.push_back(tick);

        TextCommand label;
        label.text = std::to_string(p);
        label.position = {center.x + tickHalfWidth + 15, tickY};
        label.anchor = TextAnchor::Center;
        label.fontSize = 16;
        label.color = {1.0, 1.0, 1.0, 1.0};
        list.push_back(label);
    }

    PopTransformCommand popTransform;
    list.push_back(popTransform);

    // --- NEW: bank-arc reference ticks (separate loop, different variable name) ---
    double arcRadius = region.width * 0.45;

    for (int a = -30; a <= 30; a += 10) {
        double angleRadians = a * 3.14159265358979323846 / 180.0;
        double outerX = center.x + arcRadius * std::sin(angleRadians);
        double outerY = center.y - arcRadius * std::cos(angleRadians);
        double innerX = center.x + (arcRadius - 10) * std::sin(angleRadians);
        double innerY = center.y - (arcRadius - 10) * std::cos(angleRadians);

        LineCommand arcTick;
        arcTick.start = {outerX, outerY};
        arcTick.end = {innerX, innerY};
        arcTick.width = 2.0;
        arcTick.color = {1.0, 1.0, 1.0, 1.0};
        list.push_back(arcTick);
    }

    // --- NEW: rotating bank pointer ---
    PushTransformCommand pointerRotate;
    pointerRotate.pivot = center;
    pointerRotate.rotationAngle = state.roll;
    list.push_back(pointerRotate);

    FilledPolygonCommand pointer;
    pointer.listOfPoints = {
        {center.x, center.y - (arcRadius - 20)},
        {center.x - 8, center.y - (arcRadius - 5)},
        {center.x + 8, center.y - (arcRadius - 5)}
    };
    pointer.fillColor = {1.0, 1.0, 0.0, 1.0};
    list.push_back(pointer);

    PopTransformCommand popPointerTransform;
    list.push_back(popPointerTransform);

    PopClipCommand popClip;
    list.push_back(popClip);

    return list;
}