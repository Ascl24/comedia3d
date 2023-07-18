#include "raylib.h"

int main(void) 
{
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "COMEDIA");

	Camera3D camera = { 0 };
	camera.position = (Vector3){0.0f, 10.0f, 4.0f};
	camera.target = (Vector3){0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	SetTargetFPS(60);

	while(!WindowShouldClose())
	{
		BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
				
				DrawSphere(cubePosition, 2.0f, BLUE);
				DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

				DrawGrid(10, 1.0f);

			EndMode3D();

			DrawText("Welcome to the world of SHIN MEGAMI TENSEI", 10, 40, 20, LIGHTGRAY);

			DrawFPS(10, 10);

		EndDrawing();
	}

	return 0;
}
