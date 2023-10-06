#include "raylib.h"
#include "essentials.h"
#include "src/rcamera.h"
#include "src/raymath.h"
#include "static.h"
#include "screentypes.c"
// DEFINITIONS
#ifndef MAIN
#define MAIN
#endif

// importaed from raygame
static Model model = { 0 };
static Texture2D texMapAtlas = { 0 };
static Color *mapPixels = 0;


// Forward declarations
void inputHandler();
void updateMove(Camera *camera);
void resetStatics();
void CalculateAngle();

int main(void) 
{
        // Constant definitions
        static const int screenWidth = 800;
        static const int screenHeight = 600;
        InitWindow(screenWidth, screenHeight, "COMEDIA");
        Screen SCREENSTATE = { 1 };
        MoveType MOVETYPE = { 0 }; 
        
        // Camera Parameters
        Camera3D camera = { 0 };
        camera.position = (Vector3){20.0f, 2.0f, 20.0f};
        camera.target = (Vector3){20.0f, 2.0f, 1.0f };
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        camera.fovy = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        int cameraMode = CAMERA_FIRST_PERSON;
  
        // Load textures
        Image imageMap = LoadImage("resources/game_map2.png");  // Load texMap image (RAM)
        Mesh mesh = GenMeshCubicmap(imageMap, (Vector3){ 10.0f, 6.0f, 10.0f });
        model = LoadModelFromMesh(mesh);                    // Load generated mesh into a model
        texMapAtlas = LoadTexture("resources/cubicmap_chadlas.png");      // Load map texture
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texMapAtlas;     // Set map diffuse texture
        mapPixels = LoadImageColors(imageMap); // Get map image data to be used for collision detection
        UnloadImage(imageMap);

        SetTargetFPS(60);
        while(!WindowShouldClose())
        {
                inputHandler();
                /* TODO: Create collision for walls and start creating screens */
                BeginDrawing();
                ClearBackground(RAYWHITE);
                BeginMode3D(camera);
                // draw new camera in here
                if (MOVETYPE == FREEMOVE) 
                        updateMove(&camera);
                // figure out a way to decouple frame time with movement
                DrawModel(model, origin, 1.0f, RAYWHITE);
                EndMode3D();
                // Debug text
                DrawText(TextFormat("%f %f", camera.target.x, camera.target.z), 10, 70, 20, RED);
                DrawText(TextFormat("%s", canMove ? "True" : "False"), 10, 90, 20, BLUE);
                DrawFPS(10, 10);
                EndDrawing();
                //increment the frame counter
                ++framesCounter;
        }
        UnloadModel(model);
        UnloadImageColors(mapPixels);
        UnloadTexture(texMapAtlas);
        return 0;
}
// handle inputs 
void inputHandler() 
{
        if (IsKeyDown(KEY_RIGHT)) 
                isRight = true;
        if (IsKeyDown(KEY_LEFT)) 
                isLeft = true;
        if (IsKeyDown(KEY_UP)) 
                isUp = true; 
        if (IsKeyDown(KEY_DOWN)) 
                isDown = true; 
} // cutdown on repetition
void resetStatics()
{
        canMove = false;
        currentAngle = true;
        framesCounter = 0;
}
void CalculateAngle() 
{
        if (isRight) {
                resetStatics();
                angle = -4.5f*DEG2RAD;
        }
        if (isLeft) {
                resetStatics();
                angle = 4.5f*DEG2RAD;
        }
        if (isDown) {
                resetStatics();
                angle = -9.0f*DEG2RAD;
        }
        if (isUp) {
                resetStatics();
                distance = 0.5f;
        }
        return;
}
void updateMove(Camera *camera)
{
        // Update the movement if we are in a camera handled screen
        if (canMove)
                CalculateAngle();
        if (framesCounter < 20) {
                CameraYaw(camera, angle, false);
                CameraMoveForward(camera, distance, false);
        }
        if (framesCounter > 20) {
                canMove = true;
                angle = 0.0f;
                distance = 0.0f;
                framesCounter = 0;
                isRight = isLeft = isDown = isUp = false;
        }
}
