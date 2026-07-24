#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/instruments/AttitudeIndicator.h"

TEST_CASE("AttitudeIndicator: pitch shifts the horizon down") {
    Rectangle region = {0, 0, 400, 400};

    AttitudeIndicator indicator;

    FlightState level;
    level.pitch = 0.0;
    DrawList listLevel = indicator.render(level, region);

    FlightState pitchUp;
    pitchUp.pitch = 10.0;
    DrawList listPitched = indicator.render(pitchUp, region);

    FilledPolygonCommand skyLevel;
    for (const auto& cmd : listLevel) {
        if (std::holds_alternative<FilledPolygonCommand>(cmd)) {
            skyLevel = std::get<FilledPolygonCommand>(cmd);
            break;
        }
    }

    FilledPolygonCommand skyPitch;
    for (const auto& cmd : listPitched) {
        if (std::holds_alternative<FilledPolygonCommand>(cmd)) {
            skyPitch = std::get<FilledPolygonCommand>(cmd);
            break;
        }
    }

    double horizonYLevel = skyLevel.listOfPoints[2].y;
    double horizonYPitched = skyPitch.listOfPoints[2].y;

    REQUIRE(horizonYLevel == Catch::Approx(200.0));
    REQUIRE(horizonYPitched == Catch::Approx(300.0));
}

TEST_CASE("AttitudeIndicator: roll sets the horizon's rotation angle") {
    Rectangle region = {0, 0, 400, 400};
    AttitudeIndicator indicator;

    FlightState banked;
    banked.roll = 15.0;
    DrawList list = indicator.render(banked, region);

    PushTransformCommand horizontalTransform;
    for (const auto& cmd : list) {
        if (std::holds_alternative<PushTransformCommand>(cmd)) {
            horizontalTransform = std::get<PushTransformCommand>(cmd);
            break;
        }
    }

    REQUIRE(horizontalTransform.rotationAngle == Catch::Approx(-15.0));
}