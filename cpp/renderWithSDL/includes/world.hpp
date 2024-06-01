#pragma once

#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <tuple>
#include "object.hpp"
    
class World
{
public:
	World(SDL_Renderer *inputRenderer, float inputCameraFov, int displayX, int displayY);

	void handleInput(SDL_Event const &event);

    void renderObject(Object object);

private:
	void renderTriPolygon(int x1, int y1, int z1, 
			int x2, int y2, int z2, 
			int x3, int y3, int z3);
	std::tuple<float, float> renderPointRelative(float ix,float iy,float iz);
	SDL_Renderer *renderer;
	float cameraFov;
	float cameraX;
	float cameraY;
	float cameraZ;
	float cameraYaw;
	float cameraPitch;
	float movementSpeed;
	float sensitivity;
	int disX;
	int disY;
    std::vector<Surface> mesh;
};
