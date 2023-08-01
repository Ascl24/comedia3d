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
void CollisionChecker(Camera *camera);

int main(void) 
{
	// Constant definitions
	static const int screenWidth = 800;
	static const int screenHeight = 600;
	static const Vector3 origin = {0.0f, 0.0f, 0.0f};

	Screen SCREENSTATE = { 1 };
	MoveType MOVETYPE = { 0 }; 

	InitWindow(screenWidth, screenHeight, "COMEDIA");
	
	// Camera Parameters
	Camera3D camera = { 0 };
	camera.position = (Vector3){20.0f, 2.0f, 20.0f};
	camera.target = (Vector3){20.0f, 2.0f, 1.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

  static int mapArray[32][32] = { 0 };
  
  // Load textures
  Image imMap = LoadImage("resources/game_map.png");  // Load texMap image (RAM)
  Mesh mesh = GenMeshCubicmap(imMap, (Vector3){ 10.0f, 6.0f, 10.0f });
  model = LoadModelFromMesh(mesh);                    // Load generated mesh into a model
  texMapAtlas = LoadTexture("resources/cubicmap_chadlas.png");      // Load map texture
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texMapAtlas;     // Set map diffuse texture
  mapPixels = LoadImageColors(imMap); // Get map image data to be used for collision detection

  UnloadImage(imMap);

	int cameraMode = CAMERA_FIRST_PERSON;

	SetTargetFPS(60);

	while(!WindowShouldClose())
	{
		// check for input every frame
		inputHandler();

		/* TODO: Create collision for walls and start creating screens */
		BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
				
				// draw new camera in here
				if ((isRight || isLeft || isUp || isDown) && SCREENSTATE == DUNGEON ) 
				{
					if (MOVETYPE == FREEMOVE) updateMove(&camera);
				}
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
	if (IsKeyDown(KEY_RIGHT)) isRight = true;
	if (IsKeyDown(KEY_LEFT)) isLeft = true;
	if (IsKeyDown(KEY_UP)) isUp = true; 
	if (IsKeyDown(KEY_DOWN)) isDown = true; 
	if (IsKeyDown(KEY_SPACE)) waffle += 0.01f;
	if (IsKeyDown(KEY_V)) waffle -= 0.01f;
} // cutdown on repetition
void resetStatics()
{
	canMove = false;
	currentAngle = true;
	framesCounter = 0;
}
void updateMove(Camera *camera)
{
	// Update the movement if we are in a camera handled screen
	if (currentAngle == false) angle = 0.0f;
	if (isRight && canMove) {resetStatics(); angle = -4.5f*DEG2RAD; }
	if (isLeft && canMove) {resetStatics(); angle = 4.5f*DEG2RAD; }
	if (isDown && canMove) {resetStatics(); angle = -9.0f*DEG2RAD; }
	if (isUp && canMove) {resetStatics(); distance = 0.5f; }

	if (framesCounter < 20) // movement takes half a second, or 30 frames so we check framesCounter
	{
	CameraYaw(camera, angle, false);
	CameraMoveForward(camera, distance, true);
	}
	if (framesCounter > 20) 
	{
		canMove = true;
		currentAngle = false;
		framesCounter = 0;
		angle = 0.0f;
		distance = 0.0f;
		isRight = isLeft = isDown = isUp = false;
	}
}
void CollisionChecker(Camera *camera)
{
  float playerRadius = 0.1f;
  Vector3 forward = GetCameraForward(camera);
  Vector3 wishPosition = camera->position;
  forward.y = 0;
  forward = Vector3Normalize(forward);
  forward = Vector3Scale(forward, 10.0f);
  wishPosition = Vector3Add(wishPosition, forward);
}
