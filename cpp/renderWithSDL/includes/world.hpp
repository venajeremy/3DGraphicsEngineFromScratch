#pragma once

#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <tuple>

class World
{
public:
	World(SDL_Renderer *inputRenderer, double inputCameraFov, int displayX, int displayY);

	void handleInput(SDL_Event const &event);

	void renderTriPolygon(int x1, int y1, int z1, 
			int x2, int y2, int z2, 
			int x3, int y3, int z3);
private:
	std::tuple<double, double> renderPointRelative(double ix,double iy,double iz);
	SDL_Renderer *renderer;
	double cameraFov;
	double cameraX;
	double cameraY;
	double cameraZ;
	double movementSpeed;
	int disX;
	int disY;
};
