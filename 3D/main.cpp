#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>
#include <string>

struct Moon {
    std::string name;
    float distance;      // Million km from planet
    float radius;        // 10,000 km units
    float orbitalPeriod; // Earth Days
    Color color;
    float currentAngle;
};

struct Planet {
    std::string name;
    float distance;      // Million km from Sun
    float radius;        // 10,000 km units
    float orbitalPeriod; // Earth Days
    Color color;
    float currentAngle;
    std::vector<Moon> moons;
};

int main() {
    const int screenWidth  = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "3D Solar System: True Scale");

    // --- Camera setup ---
    Camera3D camera = { { 0 } };
    camera.position   = (Vector3){ 300.0f, 250.0f, 300.0f };
    camera.target     = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Yaw/pitch derived from initial look direction
    Vector3 initDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    float yaw   = atan2f(initDir.z, initDir.x);   // horizontal angle
    float pitch = asinf(initDir.y);                // vertical angle

    DisableCursor();

    std::vector<Planet> solarSystem = {
        {"Mercury", 57.9f,   0.24f,  88.0f,    LIGHTGRAY, 0.0f, {}},
        {"Venus",   108.2f,  0.60f,  224.7f,   YELLOW,    0.0f, {}},
        {"Earth",   149.6f,  0.63f,  365.2f,   BLUE,      0.0f,
            {{"Moon", 0.384f, 0.17f, 27.3f, GRAY, 0.0f}}},
        {"Mars",    227.9f,  0.33f,  687.0f,   RED,       0.0f, {}},
        {"Jupiter", 778.6f,  6.99f,  4331.0f,  ORANGE,    0.0f,
            {{"Io",     0.42f, 0.18f, 1.76f, GOLD,  0.0f},
             {"Europa", 0.67f, 0.15f, 3.55f, WHITE, 0.0f}}},
        {"Saturn",  1433.5f, 5.82f,  10747.0f, BEIGE,     0.0f,
            {{"Titan", 1.2f, 0.25f, 15.9f, GOLD, 0.0f}}},
        {"Uranus",  2872.5f, 2.53f,  30589.0f, SKYBLUE,   0.0f, {}},
        {"Neptune", 4495.1f, 2.46f,  59800.0f, DARKBLUE,  0.0f, {}}
    };

    float sunRadius = 69.6f;
    bool cursorEnabled = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        // --- Toggle mouse look ---
        if (IsKeyPressed(KEY_M)) {
            cursorEnabled = !cursorEnabled;
            if (cursorEnabled) EnableCursor();
            else               DisableCursor();
        }

        float dt = GetFrameTime();

        // --- Mouse look (only when cursor is hidden) ---
        if (!cursorEnabled) {
            const float mouseSensitivity = 0.003f;
            Vector2 mouseDelta = GetMouseDelta();
            yaw   += mouseDelta.x * mouseSensitivity;
            pitch -= mouseDelta.y * mouseSensitivity;
            // Clamp pitch so we don't flip upside down
            if (pitch >  1.5f) pitch =  1.5f;
            if (pitch < -1.5f) pitch = -1.5f;

            // Rebuild forward vector from yaw/pitch
            Vector3 forward = {
                cosf(pitch) * cosf(yaw),
                sinf(pitch),
                cosf(pitch) * sinf(yaw)
            };
            camera.target = Vector3Add(camera.position, forward);
        }

        // --- WASD movement ---
        float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 3000.0f : 200.0f;

        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
        Vector3 moveDir = { 0, 0, 0 };

        if (IsKeyDown(KEY_W)) moveDir = Vector3Add(moveDir, forward);
        if (IsKeyDown(KEY_S)) moveDir = Vector3Subtract(moveDir, forward);
        if (IsKeyDown(KEY_D)) moveDir = Vector3Add(moveDir, right);
        if (IsKeyDown(KEY_A)) moveDir = Vector3Subtract(moveDir, right);

        // Only move if there's input (avoids normalising a zero vector)
        float moveLen = Vector3Length(moveDir);
        if (moveLen > 0.0f) {
            Vector3 velocity = Vector3Scale(Vector3Scale(moveDir, 1.0f / moveLen), speed * dt);
            camera.position = Vector3Add(camera.position, velocity);
            camera.target   = Vector3Add(camera.target,   velocity);
        }

        // --- Orbital physics (real-time: 1 second = 1 Earth day) ---
        for (auto &p : solarSystem) {
            p.currentAngle += (2.0f * PI / p.orbitalPeriod) * dt;
            for (auto &m : p.moons) {
                m.currentAngle += (2.0f * PI / m.orbitalPeriod) * dt;
            }
        }

        // --- Draw ---
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);

                // Sun
                DrawSphere({ 0, 0, 0 }, sunRadius, GOLD);
                DrawSphereWires({ 0, 0, 0 }, sunRadius + 0.5f, 16, 16, ORANGE);

                for (const auto &p : solarSystem) {
                    Vector3 pPos = {
                        cosf(p.currentAngle) * p.distance,
                        0.0f,
                        sinf(p.currentAngle) * p.distance
                    };

                    // Orbit path
                    DrawCircle3D({ 0, 0, 0 }, p.distance, { 1, 0, 0 }, 90.0f, Fade(p.color, 0.1f));

                    // Planet
                    DrawSphere(pPos, p.radius, p.color);

                    // Saturn rings
                    if (p.name == "Saturn")
                        DrawCircle3D(pPos, p.radius * 2.2f, { 1, 0, 0 }, 90.0f, Fade(p.color, 0.4f));

                    // Moons
                    for (const auto &m : p.moons) {
                        Vector3 mPos = {
                            pPos.x + cosf(m.currentAngle) * m.distance,
                            0.0f,
                            pPos.z + sinf(m.currentAngle) * m.distance
                        };
                        DrawSphere(mPos, m.radius, m.color);
                    }
                }

                DrawGrid(100, 500.0f);

            EndMode3D();

            // --- Nametags ---
            for (const auto &p : solarSystem) {
                Vector3 pPos = {
                    cosf(p.currentAngle) * p.distance,
                    p.radius + 5.0f,
                    sinf(p.currentAngle) * p.distance
                };
                Vector2 sPos = GetWorldToScreen(pPos, camera);
                // Filter out points projected behind the camera (negative w gives huge coords)
                if (sPos.x > 0 && sPos.x < screenWidth && sPos.y > 0 && sPos.y < screenHeight) {
                    DrawText(p.name.c_str(), (int)sPos.x - 20, (int)sPos.y,      15, RAYWHITE);
                    DrawText(TextFormat("%.0fM km", p.distance),
                             (int)sPos.x - 20, (int)sPos.y + 15, 10, GRAY);
                }
            }

            // --- HUD ---
            DrawRectangle(10, 10, 400, 130, Fade(DARKGRAY, 0.8f));
            DrawText("TRUE SCALE: 1s = 1 DAY",                     20, 20, 20, SKYBLUE);
            DrawText("WASD: Move  |  Mouse: Look",                  20, 48, 10, RAYWHITE);
            DrawText("Shift: Fast  |  M: Toggle cursor",            20, 62, 10, YELLOW);
            DrawText(TextFormat("Dist from Sun: %.1f M km",
                     Vector3Distance(camera.position, {0,0,0})),    20, 80, 10, RAYWHITE);
            DrawFPS(10, screenHeight - 30);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
