#include "raylib.h"
#include "essentials.h"

// DEFINITIONS
#ifndef MAIN
#define MAIN
#endif

/* Static Variables */
static int framesCounter = 0;
static bool canMove = true;
static bool currentAngle = false;
static float angle = 0.0f;
static float distance = 0.0f;
static bool isUp = false;
static bool isDown = false;
static bool isRight = false;
static bool isLeft = false;
static float waffle = 0.0f;
static bool canMoveForwards = false;
// importaed from raygame
static Model model = { 0 };
static Texture2D texMapAtlas = { 0 };
static Color *mapPixels = 0;

typedef enum Screen {
	WORLDMAP,
	DUNGEON,
	MENU
} Screen;

typedef enum MoveType {
	FREEMOVE,
	MENUMOVE
} MoveType;

// Forward declarations
void inputHandler();
void CameraYaw(Camera *camera, float angle, bool rotateAroundTarget);
void CameraMoveForward(Camera *camera, float distance, bool moveInWorldSpace);
void CameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
void updateMove(Camera *camera);
void resetStatics();

int main(void) 
{
	// Constant definitions
	static const int screenWidth = 800;
	static const int screenHeight = 600;
	static const Vector3 origin = {0.0f, 0.0f, 0.0f};
	Vector2 TILE = (Vector2){10.0f, 10.0f};

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

  // Load textures
  Image imMap = LoadImage("resources/game_map.png");  // Load texMap image (RAM)
  Mesh mesh = GenMeshCubicmap(imMap, (Vector3){ 10.0f, 6.0f, 10.0f });
  model = LoadModelFromMesh(mesh);                    // Load generated mesh into a model
  texMapAtlas = LoadTexture("resources/cubicmap_atlas.png");      // Load map texture
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texMapAtlas;     // Set map diffuse texture
  mapPixels = LoadImageColors(imMap); // Get map image data to be used for collision detection

  UnloadImage(imMap);

	int cameraMode = CAMERA_FIRST_PERSON;

	SetTargetFPS(60);

	while(!WindowShouldClose())
	{
		// check for input every frame
		inputHandler();
		// and then update camera movement


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
				// DrawGrid(10, 10.0f);
				DrawPlane(origin, (Vector2){10.0f, 10.0f}, BROWN);
				DrawPlane((Vector3){0.0f, 0.0f, 10.0f}, TILE, RED);
				DrawPlane((Vector3){10.0f, 0.0f, 0.0f}, TILE, BLUE);
				DrawPlane((Vector3){10.0f, 0.0f, 10.0f}, TILE, GREEN);
				DrawPlane((Vector3){0.0f, waffle, 0.0f}, TILE, SKYBLUE);
        DrawModel(model, origin, 1.0f, RAYWHITE);


			EndMode3D();
			
			// Debug text
			DrawText("Welcome to the world of SHIN MEGAMI TENSEI!", 10, 40, 20, LIGHTGRAY);
			DrawText(TextFormat("%f %f %f", camera.position.x, camera.position.y, camera.position.z), 10, 70, 20, RED);
			DrawText(TextFormat("%s", canMove ? "True" : "False"), 10, 90, 20, BLUE);
			DrawText(TextFormat("%f", waffle), 10, 110, 20, RED);


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
	if (isUp && canMove && canMoveForwards) {resetStatics(); distance = 0.5f; }

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
