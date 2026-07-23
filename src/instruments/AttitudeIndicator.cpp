#include "AttitudeIndicator.h"

DrawList AttitudeIndicator::render(const FlightState& state, const Rectangle& region) const {
    Point center;
    center.x = region.x + region.width / 2.0;
    center.y = region.y + region.height / 2.0;

    double pixelsPerDegree = region.height/ 40.0;
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

    return list;
}