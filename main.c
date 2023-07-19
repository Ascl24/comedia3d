#include "raylib.h"
#include "raymath.h"
#include <math.h>

/* Static Variables */
static int framesCounter = 0;
static bool canMove = true;
static bool currentAngle = false;
static float angle = 0.0f;
static float distance = 0.0f;

void customCameraUpdate(Camera3D *camera);
void CameraYaw(Camera *camera, float angle, bool rotateAroundTarget);
void CameraMoveForward(Camera *camera, float distance, bool moveInWorldSpace);
void resetStatics();
int main(void) 
{
	static const int screenWidth = 800;
	static const int screenHeight = 600;
	static const Vector3 origin = {0.0f, 0.0f, 0.0f};
	InitWindow(screenWidth, screenHeight, "COMEDIA");

	Camera3D camera = { 0 };
	camera.position = (Vector3){0.0f, 2.0f, 1.0f};
	camera.target = (Vector3){0.0f, 2.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	int cameraMode = CAMERA_FIRST_PERSON;

	SetTargetFPS(60);

	while(!WindowShouldClose())
	{
		BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
				
				// figure out a way to decouple frame time wiht movement
				customCameraUpdate(&camera);
				DrawPlane(origin, (Vector2){16.0f, 16.0f}, BROWN);

			EndMode3D();

			DrawText("Welcome to the world of SHIN MEGAMI TENSEI!", 10, 40, 20, LIGHTGRAY);
			DrawText(TextFormat("%f %f %f", camera.target.x, camera.target.y, camera.target.z), 10, 70, 20, RED);
			DrawText(TextFormat("%s", canMove ? "True" : "False"), 10, 90, 20, BLUE);

			DrawFPS(10, 10);

		EndDrawing();
		++framesCounter;
	}
	return 0;
}
void customCameraUpdate(Camera3D *camera)
{
	if (currentAngle == false) angle = 0.0f;
	if (IsKeyDown(KEY_RIGHT) && canMove) 
	{
		resetStatics();
		angle = -3.0f*DEG2RAD;
	}
	if (IsKeyDown(KEY_LEFT) && canMove) 
	{
		resetStatics();
		angle = 3.0f*DEG2RAD;
	}
	if (IsKeyDown(KEY_UP) && canMove) 
	{
		resetStatics();
		distance = 0.5f;
	}
	if (IsKeyDown(KEY_DOWN) && canMove) 
	{
		resetStatics();
		angle = -6.0f*DEG2RAD;
	}
	if (framesCounter < 30) 
	{
	CameraYaw(camera, angle, false);
	CameraMoveForward(camera, distance, true);
	}
	if (framesCounter > 30) 
	{
		canMove = true;
		currentAngle = false;
		framesCounter = 0;
		angle = 0.0f;
		distance = 0.0f;
	}
}
void resetStatics()
{
	canMove = false;
	currentAngle = true;
	framesCounter = 0;
}
