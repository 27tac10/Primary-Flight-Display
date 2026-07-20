# Primary Flight Display (PFD) — Design

**Date:** 2026-07-20
**Status:** Approved design, ready for implementation planning
**Language:** C++17 · **Build:** CMake · **First backend:** SDL2 + SDL2_ttf (+ SDL2_gfx)

## 1. Purpose & Goals

A desktop Primary Flight Display application, built in C++, as a portfolio
project. It renders the core instruments of a glass-cockpit PFD driven by an
internal flight-data simulator — self-contained and demoable anywhere, with no
external simulator or hardware required.

The project has an explicit **"start simple, level up"** goal: begin with an
SDL2 rendering backend, then replace it with an **OpenGL** backend. OpenGL is
the deliberate target because the avionics industry standardizes on **OpenGL SC
(Safety Critical)** — a certified subset of OpenGL — so an SDL2 → OpenGL
progression moves toward the real industry stack (OpenGL SC, ARINC 661, tools
like Ansys SCADE Display / Presagis VAPS XT / DiSTI GL Studio).

### Success criteria

- A window opens showing a recognizable PFD: attitude indicator (center),
  airspeed tape (left), altitude tape (right), heading tape (bottom).
- Instruments animate correctly from simulated flight data at ~60 fps.
- The rendering backend can be swapped (SDL2 → OpenGL later) **without changing
  any instrument or flight-logic code**.
- Instrument geometry and the simulator are unit-tested with no window open.

### Non-goals (YAGNI for v1)

- No real flight-sim (X-Plane/MSFS) or hardware/sensor integration.
- No vertical-speed indicator, slip/skid ball, flight director, or autopilot
  annunciations. (Possible later; not v1.)
- No general-purpose 2D graphics API — only the primitives the four instruments
  need.
- No multithreading, networking, or persistence.

## 2. Instruments (v1 scope)

| Instrument | Position | Shows |
|---|---|---|
| **Attitude Indicator** (core) | Center | Artificial horizon (sky/ground), pitch ladder, bank arc + pointer |
| **Airspeed Tape** | Left | Vertical scrolling tape + boxed rolling readout |
| **Altitude Tape** | Right | Vertical scrolling tape + boxed rolling readout |
| **Heading Indicator** | Bottom | Horizontal scrolling heading tape + current-heading box |

The attitude indicator is included as the non-negotiable core — the tapes and
heading are arranged around it; a PFD without an artificial horizon is not a PFD.

## 3. Architecture

Five layers. The domain (flight data + instrument geometry) is fully isolated
from the rendering backend, which is the only part that will change when
leveling up.

```
Application  (fixed-timestep loop, input handling, timing)
   │  each frame:
   ├── IDataSource.update(dt) ──► FlightState   (pitch, roll, airspeed, altitude, heading, verticalSpeed)
   │        └── SimulatedDataSource  (scripted demo profile + optional keyboard control)
   │
   ├── Layout(windowSize) ──► region (rectangle) per instrument
   │
   ├── Instruments: render(FlightState, region) ──► DrawList     ← pure, unit-tested, no window
   │        ├── AttitudeIndicator
   │        ├── AirspeedTape
   │        ├── AltitudeTape
   │        └── HeadingIndicator
   │
   └── Backend.render(combined DrawList)      ← ONLY layer that knows SDL/OpenGL
            └── SdlBackend   (later: GlBackend, QtBackend)
```

### 3.1 The key design decision: `DrawList`, not a per-call renderer interface

A naive `IRenderer` with virtual `drawLine()`/`drawPolygon()`/`text()` per call
is **rejected** because it abstracts at the wrong level and would make leveling
up *harder*:

- SDL2's native primitives are weak (no filled polygons, no AA/thick lines).
- OpenGL is retained/batched — wrapping it behind per-call `drawLine` discards
  the reason to use it and is harder than using it directly.
- Qt's QPainter has its own transform/scene model that fights a per-call API.
- Text/font handling would get locked to the first backend's font model.

**Chosen instead:** each instrument emits a **`DrawList`** — a vector of
value-typed draw commands (plain data). A **`Backend`** consumes the whole list
and renders it using its framework's native idiom (SDL immediate, OpenGL
batched, Qt QPainter).

Benefits:
- **Testing is simpler, not harder:** the emitted command vector *is* the test
  surface. No mock renderer needed.
- **Leveling up actually works:** each backend gets a batch and uses its native
  approach.
- **No font coupling:** a text command carries `{text, position, size, color}`;
  each backend resolves fonts its own way (SDL_ttf / GL texture atlas / QFont).

### 3.2 Coordinate space

Instruments draw in a **normalized/logical space** relative to their assigned
region (e.g. their region mapped to a fixed virtual coordinate space), never in
raw window pixels. The **backend** applies the region→pixel transform and
handles DPI and window resize.

- Keeps instruments resolution-independent (resize just works).
- Keeps geometry math clean (pitch° → normalized offset, independent of pixel
  size).
- Avoids coupling to a backend's coordinate convention (SDL top-left pixels vs
  OpenGL bottom-left/NDC).

### 3.3 Draw command vocabulary (small and deliberate)

Only what the four instruments need — implementable across SDL2, OpenGL, and Qt:

| Command | Fields | Used by |
|---|---|---|
| `FilledPolygon` | points, color | horizon sky/ground, boxes, pointers |
| `Line` | p0, p1, width, color | pitch ladder, tick marks, arc segments |
| `Text` | string, position, anchor, size, color | tape numbers, readouts, headings |
| `PushClip` / `PopClip` | rect | clip ladder to attitude window; clip scrolling tapes |
| `Transform` (push/pop) | 2D transform (translate/rotate/scale) | rotate the attitude ball by roll angle |

YAGNI: no gradients, curves, images, or textures in the command set. The SDL
backend may *internally* use SDL2_gfx or a rotated texture to satisfy
`FilledPolygon`/rotation, but that never leaks into instruments.

Clipping is a first-class command because the pitch ladder must clip to the
attitude window and the tapes scroll inside fixed frames — and clipping differs
per backend (`SDL_RenderSetClipRect` vs `glScissor` vs `QPainter::setClipRect`).

### 3.4 Components and their contracts

- **`FlightState`** — plain struct; the single shared vocabulary between data
  and display. Fields: `pitch`, `roll`, `airspeed`, `altitude`, `heading`,
  `verticalSpeed`. SI/aviation units documented in code.
- **`IDataSource`** — `FlightState update(double dt)`. The seam for future data
  sources (real sim, hardware). One implementation in v1.
  - **`SimulatedDataSource`** — integrates a gentle scripted demo profile
    (frame-rate-independent via `dt`); optional keyboard control to sweep
    pitch/roll/airspeed/etc. for live demos.
- **`Layout`** — pure function/struct: `(windowSize) → { region per instrument }`.
  Standard PFD placement (attitude center, airspeed left, altitude right,
  heading bottom). All positioning logic lives here.
- **`Instrument`** (interface) — `DrawList render(const FlightState&, Region) const`.
  Each concrete instrument is self-contained: input `(state, region)`, output
  `DrawList`, zero cross-instrument coupling.
- **`Backend`** (interface) — `render(const DrawList&)` plus frame begin/end and
  region→pixel/DPI handling. Only layer that includes SDL/OpenGL headers.
  - **`SdlBackend`** — SDL2 + SDL2_ttf (+ SDL2_gfx for filled polygons / the
    rotated horizon).
- **`Application`** — owns the window, the fixed-timestep loop, input, and wires
  data source → layout → instruments → backend.

## 4. Data & Control Flow (per frame)

1. `Application` computes `dt`; polls SDL events (quit, optional keyboard input).
2. `SimulatedDataSource.update(dt)` → current `FlightState`.
3. `Layout(windowSize)` → each instrument's region.
4. Each instrument `.render(state, region)` → its `DrawList`; lists are combined.
5. `Backend.render(combinedDrawList)` maps logical coords → pixels and draws.
6. Present; sleep/vsync to hold ~60 fps.

## 5. Error Handling

- **Fail fast on startup** with a clear message: SDL init, window creation, and
  font load. If a resource is missing the app exits with a readable error, not a
  crash.
- **The render loop is total:** out-of-range values are clamped (e.g. pitch to
  [-90, 90], roll wraps, heading wraps 0–360) rather than throwing or crashing.
- Backend draw calls never propagate exceptions into instrument logic —
  instruments produce data only.

## 6. Testing Strategy

Because instruments emit plain `DrawList` data and the simulator is pure, most
logic is tested headless with no window:

- **`FlightState` / units** — construction, ranges.
- **`SimulatedDataSource`** — deterministic given a seed/profile and `dt`;
  frame-rate independence; value ranges stay valid.
- **Instrument geometry** — the math that matters: pitch° → ladder pixel offset,
  airspeed/altitude → tape offset, heading → tape offset, roll → bank-pointer
  angle, clipping rects. Assert on the emitted `DrawList` (positions/counts).
- **`Layout`** — regions are correct and non-overlapping for several window
  sizes.
- Backends (`SdlBackend`) are exercised manually/visually, not unit-tested — the
  testable logic has been pushed out of them by design.

Test framework: a lightweight C++ framework (e.g. Catch2 or GoogleTest) wired
through CMake/CTest.

## 7. Project Layout

```
CMakeLists.txt
src/
  core/
    FlightState.h
    IDataSource.h
    SimulatedDataSource.h / .cpp
    Layout.h / .cpp
  render/
    DrawCommand.h        (DrawList + command types)
    Backend.h            (interface)
    SdlBackend.h / .cpp
  instruments/
    Instrument.h         (interface)
    AttitudeIndicator.h / .cpp
    AirspeedTape.h / .cpp
    AltitudeTape.h / .cpp
    HeadingIndicator.h / .cpp
  app/
    Application.h / .cpp
    main.cpp
tests/
  ...
assets/
  fonts/                 (a bundled TTF for SDL_ttf)
```

## 8. Level-Up Path (planned, not v1)

- Add **`GlBackend`** (OpenGL): consumes the same `DrawList`, batches geometry,
  renders text via a texture atlas. No instrument or flight-logic changes — this
  is the payoff of the `DrawList`/`Backend` split, and it moves the project
  toward the real avionics stack (OpenGL SC).
- Possible further instruments: vertical-speed indicator, slip/skid, flight
  director.

### Known accepted caveats

- **Plain SDL2 cannot fill arbitrary polygons or anti-alias** — the SDL backend
  pulls in SDL2_gfx (or renders the horizon via a rotated texture). Contained
  entirely within `SdlBackend`.
- **macOS deprecates OpenGL** (frozen at 4.1) in favor of Metal; it still works
  for the OpenGL level-up on the dev machine, but is legacy there. SDL2 works
  fully on macOS.
- **Vulkan / Vulkan SC** is the modern successor and is emerging in avionics,
  but is out of scope — OpenGL is today's avionics standard and the right target
  here.
```
