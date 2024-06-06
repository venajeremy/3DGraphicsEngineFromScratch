#pragma once

#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <tuple>
#include <vector>
#include "object.hpp"
    
class World
{
public:
	World(SDL_Renderer *inputRenderer, float inputCameraFov, int displayX, int displayY);

	void handleInput(SDL_Event const &event);

    void addObject(Object object);

    void renderWorld();

private:
    void handleMovement(float dX, float dY, float dZ);
    void handleRotation(float dYaw, float dPitch, float dRoll);

    void renderObject(Object object);
	void renderTriPolygon(float x1, float y1, float z1, 
			float x2, float y2, float z2, 
			float x3, float y3, float z3, std::array<float,4> color);
    void renderEdgeTriPolygon(float x1, float y1, float z1, 
			float x2, float y2, float z2, 
			float x3, float y3, float z3, std::array<float,4> color);
    void rasterizePix(SDL_Renderer *renderer, int pixX, int pixY, int x1, int y1, int x2, int y2, int x3, int y3);

	std::tuple<int, int> renderPointRelative(float ix,float iy,float iz);

	SDL_Renderer *renderer;

    std::vector<Object> objects;

    std::vector<float> zBuffer;
    std::vector<float> emptyBuffer;

    float smallestX;
    float smallestY;
    float greatestX;
    float greatestY;

    float screenX1;
    float screenX2;
    float screenX3;
    float screenY1;
    float screenY2;
    float screenY3;

    int topLeftPixScreenX;
    int topLeftPixScreenY;
    int topLeftPixPerspZ;

    float currZ;
    float pixZ;

    float slope1;
    float slope2;
    float slope3;

    int crossCount;

    float zCalcDenominator;

	float cameraFov;
	float cameraX;
	float cameraY;
	float cameraZ;
	float cameraYaw;
	float cameraPitch;
	float movementSpeed;
	float sensitivity;
    float dY;
    float dP;
	int disX;
	int disY;

    std::vector<Surface> mesh;
};
