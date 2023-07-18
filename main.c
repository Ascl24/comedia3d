#include "raylib.h"

int main(void) 
{
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "COMEDIA");

	while(!WindowShouldClose())
	{
		BeginDrawing();
			ClearBackground(RAYWHITE);	
		EndDrawing();
	}

	return 0;
}
