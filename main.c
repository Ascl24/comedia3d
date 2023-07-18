#include "raylib.h"

void updateCameraTarget(Vector3 *cameratarget); 
int main(void) 
{
	const int screenWidth = 800;
	const int screenHeight = 600;
	const Vector3 origin = {0.0f, 0.0f, 0.0f};
	bool canMove;

	InitWindow(screenWidth, screenHeight, "COMEDIA");

	Camera3D camera = { 0 };
	camera.position = (Vector3){0.0f, 2.0f, 4.0f};
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
				updateCameraTarget(&camera.target);
				DrawPlane(origin, (Vector2){16.0f, 16.0f}, BROWN);

			EndMode3D();

			DrawText("Welcome to the world of SHIN MEGAMI TENSEI", 10, 40, 20, LIGHTGRAY);

			DrawFPS(10, 10);

		EndDrawing();
	}
	return 0;
}
void updateCameraTarget(Vector3 *cameratarget)
{
	if (IsKeyDown(KEY_LEFT)) cameratarget->x -= 0.5f;
	if (IsKeyDown(KEY_RIGHT)) cameratarget->x += 0.5f;
}
