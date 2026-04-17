================================================================================
                           SOLAR SYSTEM SIMULATOR
                          Orbital Mechanics Engine
================================================================================

1. OVERVIEW
-----------
This application is a 2D N-body orbital simulation built in C++ using the 
SFML (Simple and Fast Multimedia Library). It provides a high-fidelity 
representation of celestial bodies governed by Newtonian gravity, featuring 
interactive camera controls and time-dilation capabilities.

2. SYSTEM FEATURES
------------------
* Physics Engine: Implements Newton's Law of Universal Gravitation with 
  numerical sub-stepping for enhanced stability.
* Visuals: Dynamic orbital path indicators (static), scaled planetary 
  representations, and a real-time UI monitor.
* Interaction: 
    - Smooth zoom and pan functionality.
    - Click-to-focus locking on specific celestial bodies.
    - Variable time-warp levels (0x to 100x).

3. TECHNICAL SPECIFICATIONS
---------------------------
* Language: C++17
* Graphics Library: SFML 2.5+
* Dependencies: 
    - RigidBody.h (Physics state container)
    - Vector2.h (Vector mathematics)
* Performance: 60 FPS capped with 15 physics sub-steps per frame.

4. USER CONTROLS
----------------
[ MOTION ]
- Pan Camera:      Left-Click + Drag (on empty space)
- Zoom In/Out:     [ + ] and [ - ] keys
- Reset View:      [ M ] key (Return to Sun/Home)

[ SIMULATION ]
- Focus Planet:    Left-Click directly on a planet
- Increase Warp:   [ . ] (Period)
- Decrease Warp:   [ , ] (Comma)

5. PHYSICS LOGIC
----------------
The simulation initializes planets using the circular orbit velocity formula:
    v = sqrt(G * M_sun / distance)

To maintain accuracy during high time-warp settings, the engine executes 
multiple physics iterations per render frame. This prevents the "drift" 
commonly associated with Euler integration in high-velocity systems.

6. INSTALLATION & COMPILATION
-----------------------------
Ensure SFML is installed on your system. 

Compilation example (GCC):
g++ -c main.cpp -o main.o
g++ main.o -o SolarSim -lsfml-graphics -lsfml-window -lsfml-system

Ensure that a valid .ttf font file is present in the working directory 
or specified system path for the UI overlay to render.

