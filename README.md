# PhysEngine — Solar System Simulator

A C++ solar system simulation project with two independent implementations: a **2D orbital mechanics simulator** built with SFML, and a **3D first-person explorer** built with raylib. Both use real astronomical data and run at a simulation speed of 1 second = 1 Earth day.

---

## 2D Simulator (`2D/`)

A top-down orbital mechanics simulation using real Newtonian gravity computed every frame. Planets orbit the Sun under continuous gravitational acceleration, with adaptive sub-stepping to maintain orbit stability at high warp speeds.

### Physics

The gravitational constant G is derived analytically from Earth's known orbital period and distance rather than hardcoded:

```
G = (4π² * earthDist³) / (YEAR² * sunMass)
```

Each planet's initial orbital velocity is set from the circular orbit condition:

```
v = sqrt(G * M / r)
```

Every frame, gravitational acceleration is computed from the inverse-square law and integrated forward using Euler integration. At high warp speeds, the timestep is subdivided into up to 100 sub-steps per frame to prevent orbits from becoming numerically unstable:

```cpp
int steps = (timeMultiplier > 100.0f) ? 100 : 25;
float dt = (SECONDS_PER_DAY / 60.0f) * timeMultiplier / steps;
```

### Features

- Real gravitational physics — G derived from Earth's orbital period and distance
- All 8 planets with true relative distances and colour-coded labels
- Dotted orbit path indicators drawn at startup for each planet
- Time warp — 8 levels from paused up to 1000× speed
- Adaptive physics sub-stepping — maintains orbit stability at high warp
- Click-to-focus — click any planet to lock the camera onto it
- Pan and zoom — drag to pan, keyboard to zoom
- HUD overlay showing current warp, focus target, and controls

### Controls

| Key / Input | Action |
|-------------|--------|
| Left click planet | Focus camera on planet |
| Left click + drag | Pan camera |
| `+` / `-` | Zoom in / out |
| `.` / `,` | Increase / decrease time warp |
| `M` | Reset camera to default view |

### Requirements

- C++20 compiler (e.g. `g++`)
- SFML (Graphics, Window, System modules)

```bash
# macOS
brew install sfml

# Ubuntu/Debian
sudo apt install libsfml-dev
```

### Building & Running

```bash
cd 2D
make
./PhysEngine
```

---

## 3D Simulator (`3D/`)

A free-roaming first-person explorer of the solar system rendered in 3D using raylib. Move through space at any scale and watch the planets orbit in real time. Includes 2000 randomly placed background stars, moons, Saturn's rings, and floating name/distance labels for each planet.

### Physics

Orbital motion uses analytic angle integration — each planet's angle advances per frame based on its real orbital period:

```
angle += (2π / orbitalPeriod) * dt
```

Planet and moon positions are computed from their current angle each frame:

```
position = (cos(angle) * distance, 0, sin(angle) * distance)
```

### Features

- True-scale distances and sizes — planetary distances in millions of km, radii in 10,000 km units
- Real orbital periods — 1 second = 1 Earth day
- All 8 planets with colour-coded spheres and floating name/distance labels
- Moons — Earth's Moon, Jupiter's Io & Europa, and Saturn's Titan
- Saturn's rings rendered as a scaled disc
- 2000 randomly placed background stars
- Free-look first-person camera with WASD movement and mouse look
- Shift to move faster through true-scale distances
- HUD showing your current distance from the Sun in million km
- Adjustable near/far clip planes (0.1 to 10,000 units) to handle true-scale rendering

### Controls

| Key / Input | Action |
|-------------|--------|
| `W` `A` `S` `D` | Move camera |
| Mouse | Look around |
| Left Shift (held) | Move faster |
| `M` | Toggle mouse cursor |
| `Esc` | Quit |

### Requirements

- C++20 compiler (e.g. `g++`)
- raylib

```bash
# macOS
brew install raylib
```

### Building & Running

```bash
cd 3D
make
./PhysEngine
```

> **Note:** The Makefile targets macOS (Apple Silicon) with Homebrew raylib at `/opt/homebrew/opt/raylib`. On Linux, update the `CXXFLAGS` include path and remove the `-framework` flags from `LDFLAGS`.

---

## Project Structure

```
PhysEngine/
├── 2D/
│   ├── main.cpp          # Simulation logic and SFML rendering
│   ├── RigidBody.h       # Physics body definition
│   ├── RigidBody.cpp     # Physics body implementation
│   ├── vector2.h         # Custom 2D vector math
│   └── Makefile
├── 3D/
│   ├── main.cpp          # Simulation logic and raylib rendering
│   └── Makefile
└── LICENSE
```

---

## License

This project is licensed under the terms found in the LICENSE file.
