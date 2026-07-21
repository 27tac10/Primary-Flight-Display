#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/core/SimulatedDataSource.h"

TEST_CASE("SimulatedDataSource update returns valid FlightState") {
    SimulatedDataSource testDataSourceSmall;
    SimulatedDataSource testDataSourceLarge;

    FlightState stateSmall;
    FlightState stateLarge;


    for (int i = 0; i < 625; ++i) {
        stateSmall = testDataSourceSmall.update(0.016); // Simulate 0.016 seconds elapsed

        REQUIRE(stateSmall.pitch >= -90.0);
        REQUIRE(stateSmall.pitch <= 90.0);
        REQUIRE(stateSmall.roll >= -180.0);
        REQUIRE(stateSmall.roll <= 180.0);
        REQUIRE(stateSmall.heading >= 0.0);
        REQUIRE(stateSmall.heading < 360.0);
        REQUIRE(stateSmall.airspeed >= 0.0);
        REQUIRE(stateSmall.altitude >= 0.0);
    }

    for (int i = 0; i < 100; ++i) {
        stateLarge = testDataSourceLarge.update(0.1); // Simulate 0.1 seconds elapsed

        REQUIRE(stateLarge.pitch >= -90.0);
        REQUIRE(stateLarge.pitch <= 90.0);
        REQUIRE(stateLarge.roll >= -180.0);
        REQUIRE(stateLarge.roll <= 180.0);
        REQUIRE(stateLarge.heading >= 0.0);
        REQUIRE(stateLarge.heading < 360.0);
        REQUIRE(stateLarge.airspeed >= 0.0);
        REQUIRE(stateLarge.altitude >= 0.0);
    }

    REQUIRE(stateSmall.pitch == Catch::Approx(stateLarge.pitch));
    REQUIRE(stateSmall.roll == Catch::Approx(stateLarge.roll));
    REQUIRE(stateSmall.heading == Catch::Approx(stateLarge.heading));
    REQUIRE(stateSmall.airspeed == Catch::Approx(stateLarge.airspeed));
    REQUIRE(stateSmall.altitude == Catch::Approx(stateLarge.altitude));
}

