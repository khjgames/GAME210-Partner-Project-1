#pragma once
#include "SDL.h"
#include <string>

using namespace std;

struct Colour
{
	int r;
	int g;
	int b;
};

struct Vec2D
{
	float x;
	float y;
};

struct Transform2D
{
	Vec2D position;
	Vec2D scale;
	Vec2D rotation;
};

