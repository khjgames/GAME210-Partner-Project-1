#pragma once

#include "SDL.h"
#include "../Source/Objects/ObjData.h"

class Graphics
{
public:
	// should be called only once on boot.
	static bool Init();

	// needs to be called once at the start and end of game loops rendering
	static void StartRender();
	static void EndRender();

	//drawing specific functions
	static void SetColor(Colour color);
	static void DrawRect(Transform2D transform, Vec2D dimensions, bool isFilled = false);
	static void DrawLine(Transform2D transform, Vec2D dimensions);
	static void DrawText(const char* text, float x, float y, int width, int height);
private:
	// some SDL specific variables:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
};

