#include "raylib.h"

// DEFINITIONS
#define TILESIZE 10

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
void updateMove(Camera *camera);
void resetStatics();

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
	camera.position = (Vector3){0.0f, 2.0f, 10.0f};
	camera.target = (Vector3){0.0f, 2.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	int cameraMode = CAMERA_FIRST_PERSON;

	SetTargetFPS(60);

	while(!WindowShouldClose())
	{
		// check for input every frame
		inputHandler();

		// and then update camera movement


		/*  TODO: Add tiles for the ground, and start creating walls
		TODO: Create collision for walls and start creating screens */
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
				DrawPlane((Vector3){0.0f, 0.0f, 10.0f}, (Vector2){10.0f, 10.0f}, RED);

			EndMode3D();
			
			// Debug text
			DrawText("Welcome to the world of SHIN MEGAMI TENSEI!", 10, 40, 20, LIGHTGRAY);
			DrawText(TextFormat("%f %f %f", camera.target.x, camera.target.y, camera.target.z), 10, 70, 20, RED);
			DrawText(TextFormat("%s", canMove ? "True" : "False"), 10, 90, 20, BLUE);

			DrawFPS(10, 10);

		EndDrawing();

		//increment the frame counter
		++framesCounter;
	}
	return 0;
}
// handle inputs 
void inputHandler()
{
	if (IsKeyDown(KEY_RIGHT)) isRight = true;
	if (IsKeyDown(KEY_LEFT)) isLeft = true;
	if (IsKeyDown(KEY_UP)) isUp = true; 
	if (IsKeyDown(KEY_DOWN)) isDown = true; 
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
