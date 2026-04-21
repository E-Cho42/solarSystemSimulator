PhysEngine — Solar System Simulator
A C++ solar system simulation project with two independent implementations: a 2D orbital physics simulator built with SFML, and a 3D first-person explorer built with raylib. Both use real astronomical data and run at a simulation speed of 1 second = 1 Earth day.

2D Simulator (2D/)
A top-down orbital mechanics simulation using real gravitational physics. Planets orbit the Sun using Newtonian gravity computed each frame, with adaptive sub-stepping to keep orbits stable at high warp speeds.
Features

Real gravitational physics — gravitational constant derived from Earth's orbital period and distance
All 8 planets with true relative distances, colour-coded and labelled
Dotted orbit path indicators drawn at startup for each planet
Time warp — 8 warp levels from paused up to 1000× speed
Adaptive physics stepping — extra sub-steps at high warp to maintain orbit stability
Click-to-focus — click any planet to lock the camera onto it
Pan and zoom — drag to pan, keyboard to zoom
HUD overlay showing current warp, focus target, and controls

Controls
Key / InputActionLeft click planetFocus camera on planetLeft click + dragPan camera+ / -Zoom in / out. / ,Increase / decrease time warpMReset camera to default view
Requirements

C++20 compiler (e.g. g++)
SFML (Graphics, Window, System modules)

bash# macOS
brew install sfml

# Ubuntu/Debian
sudo apt install libsfml-dev
Building & Running
bashcd 2D
make
./PhysEngine

3D Simulator (3D/)
A free-roaming first-person explorer of the solar system rendered in 3D. Move through space at any scale and watch the planets orbit in real time.
Features

True-scale distances and sizes — planetary distances in millions of km, radii in 10,000 km units
Real orbital periods — 1 second = 1 Earth day
All 8 planets with colour-coded spheres and floating name/distance labels
Moons — Earth's Moon, Jupiter's Io & Europa, and Saturn's Titan
Saturn's rings rendered as a scaled disc
Free-look first-person camera with WASD movement and mouse look
HUD overlay showing controls and your current distance from the Sun

Controls
Key / InputActionW A S DMove cameraMouseLook aroundLeft Shift (held)Move fasterMToggle mouse cursorEscQuit
Requirements

C++20 compiler (e.g. g++)
raylib — install via Homebrew:

bashbrew install raylib
Building & Running
bashcd 3D
make
./PhysEngine

Note: The Makefile targets macOS (Apple Silicon) with Homebrew raylib at /opt/homebrew/opt/raylib. On Linux, update the CXXFLAGS include path and remove the -framework flags from LDFLAGS.


Project Structure
PhysEngine/
├── 2D/
│   ├── main.cpp          # Simulation logic and SFML rendering
│   ├── RigidBody.h       # Physics body definition
│   ├── RidgidBody.cpp    # Physics body implementation
│   ├── vector2.h         # 2D vector math
│   └── Makefile
├── 3D/
│   ├── main.cpp          # Simulation logic and raylib rendering
│   └── Makefile
└── LICENSE

License
This project is licensed under the terms found in the LICENSE file.