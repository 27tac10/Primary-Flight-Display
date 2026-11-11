# Primary Flight Display (PFD)

A C++ Primary Flight Display, built up step by step. The current version is
**text-only** (`TextPFD`): it prints the flight instruments to the terminal so the class
design can be checked before any graphics are added.

## Example output

```
======= Text PFD ========
Flight        : NH123
Attitude      : pitch +2.5 deg   bank -10.0 deg
Airspeed      : 250 kt
Altitude      : 10000 ft
Vertical speed: +500 ft/min
Heading       : 090 deg
Turn/slip     : +0.0 (coordinated)
Ground speed  : 265 kt
=========================
```

## Instruments

| Instrument | Field(s) | Unit |
|---|---|---|
| Attitude indicator | `pitch`, `bank` | deg (pitch + = nose up, bank + = right wing down) |
| Airspeed | `airspeed` | kt |
| Altitude | `altitude` | ft |
| Vertical speed | `verticalSpeed` | ft/min (+ = climbing) |
| Heading | `heading` | deg, wrapped to 0–360 |
| Turn/slip | `slip` | 0 = coordinated, − = slip left, + = skid right |

`groundSpeed` and `flightNumber` are also stored and shown, though a real PFD
would not normally display them.

## Architecture

```
main ──creates──► Aircraft ──owns──► FlightData
  │                  ▲
  └──creates──► TextPFD ─┘ reads (const reference)
```

- **`FlightData`** (`FlightData.h`): plain struct holding the current flight state.
- **`Aircraft`** (`Aircraft.h/.cpp`): core class. Owns the single `FlightData`;
  every change goes through its setters. Exposes read-only `getData()`.
- **`TextPFD`** (`TextPFD.h/.cpp`): display only. Holds a `const Aircraft&` and prints
  the instruments in `display()`. It stores no flight data of its own.
- **`main`** (`main.cpp`): wires them together. Sets hard-coded values,
  displays, changes some values through `Aircraft`, and displays again to show
  the TextPFD reads live data.

`Aircraft` and `TextPFD` are deliberately independent (model/view separation), so
the display can be replaced, or more displays added, without touching
`Aircraft`.

## Build & run

Requires a C++23 compiler with `<format>` support (e.g. recent Clang or GCC).

```bash
make
```

```bash
./main
```

```bash
make clean
```

## Roadmap

- Change flight data over time (e.g. an `Aircraft::update(double dt)` called
  from a loop in `main`).
- Move the wiring into a `Simulator`/`Cockpit` class once `main` grows.
- Add a graphical PFD (SFML: attitude indicator, airspeed and altitude tapes) alongside `TextPFD`.
