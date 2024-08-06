#pragma once

#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <tuple>
#include <vector>
#include <string>
#include "object.hpp"
#include "texture.hpp"
    
class World
{
public:
	World(SDL_Renderer *inputRenderer, float inputCameraFov, int displayX, int displayY);

	void handleInput(SDL_Event const &event);

    void addObject(Object object);

    void renderWorld();

    

private:
    void handleMovement(float forwards, float upwards, float sideways);
    void handleRotation(float dYaw, float dPitch, float dRoll);

    void renderObject(Object object);
	void renderTriPolygon(float x1, float y1, float z1, 
			float x2, float y2, float z2, 
			float x3, float y3, float z3, 
            float u1, float v1,
            float u2, float v2,
            float u3, float v3,
            std::string textureMap);
    void renderEdgeTriPolygon(float x1, float y1, float z1, 
			float x2, float y2, float z2, 
			float x3, float y3, float z3, std::array<float,4> color);
            
    bool compairObjectDistance(Object object1, Object object2);
    

	std::tuple<int, int> renderPointRelative(float ix,float iy,float iz);

	SDL_Renderer *renderer;

    std::vector<Object> objects;

    std::vector<float> zBuffer;
    std::vector<float> emptyBuffer;

    std::string textureFolderLocation;

    tgaImage dTex;

    float currZ1;
    float currZ2;
    float currZ3;

    int smallestY;
    int greatestY;

    int screenX1;
    int screenX2;
    int screenX3;
    int screenY1;
    int screenY2;
    int screenY3;

    int triangleEdgeLeft;
    int triangleEdgeRight;

    float currZ;
    float pixZ;
    float uRpixZ;

    float slope1;
    float slope2;
    float slope3;

    int xPos;

    float largestZ;
    float smallestZ;

    int xPos1;
    int xPos2;
    int xPos3;

    float tu_a;
    float tv_a;
    float tu_b;
    float tv_b;
    float tu_c;
    float tv_c;

    float tU;
    float tV;

    pixel pixTex;

    float a;
    float b;
    float c;

    float cameraFov;
    float cameraX;
    float cameraY;
    float cameraZ;
    float cameraYaw;
    float cameraPitch;
    float cameraRoll;
    float movementSpeed;
    float sensitivity;
    float dY;
    float dP;
    int disX;
	int disY;

    std::vector<Surface> mesh;};
