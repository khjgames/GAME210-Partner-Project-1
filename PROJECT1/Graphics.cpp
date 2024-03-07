#include "Graphics.h"
#include <SDL_ttf.h>

SDL_Window* Graphics::window;
SDL_Renderer* Graphics::renderer;

bool Graphics::Init()
{
	// create little window with minimize and x to close 
	window = SDL_CreateWindow("Partner Project 1",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (!window)
	{
		return false;
	}

	// fill in that window with a virtual canvas to draw on called a renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		return false;
	}

	TTF_Init();

	return true;
}

void Graphics::StartRender()
{
	// clear the screen color
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
}

void Graphics::SetColor(Colour color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
}

void Graphics::DrawRect(Transform2D transform, Vec2D dimensions, bool isFilled)
{
	SDL_Rect Bounds;
	Bounds.x = transform.position.x;
	Bounds.y = transform.position.y;
	Bounds.w = dimensions.x;
	Bounds.h = dimensions.y;

	if (isFilled)
	{
		SDL_RenderFillRect(renderer, &Bounds);
	}
	else
	{
		SDL_RenderDrawRect(renderer, &Bounds);
	}
}

void Graphics::DrawLine(Transform2D transform, Vec2D dimensions)
{
	SDL_Rect Bounds;
	Bounds.x = transform.position.x;
	Bounds.y = transform.position.y;
	Bounds.w = dimensions.x;
	Bounds.h = dimensions.y;

	SDL_RenderDrawLine(renderer, transform.position.x, transform.position.y, dimensions.x, dimensions.y);
}

void Graphics::EndRender() 
{
	SDL_RenderPresent(renderer);
	// wait 2 frames
	SDL_Delay(2);
}

// example based on the code from: https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
void Graphics::DrawText(const char * text, float x, float y, int width, int height)
{
	TTF_Font* font = TTF_OpenFont("arialbd.ttf", 24);

	SDL_Color Red = { 255, 0, 0 };

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, Red);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect Message_rect;
	Message_rect.x = x;  
	Message_rect.y = y; 
	Message_rect.w = width; 
	Message_rect.h = height; 

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}
