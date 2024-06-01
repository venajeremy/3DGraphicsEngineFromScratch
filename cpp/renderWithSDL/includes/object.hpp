#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <math.h>

class Surface
{
public:
    Surface(std::array<float,9> inVerticies);
    std::array<float,9> vertices;
    std::array<float,4> color;
private:
     
};

class Object 
{
public:
    Object(std::vector<Surface> inMesh, float inPosX, float inPosY, float inPosZ, float inYaw, float inPitch, float inRoll);
    
    void worldTranslate(float dX, float dY, float dZ);
    void worldRotate(float dYaw, float dPitch, float dRoll);

    std::vector<Surface> getMesh();

private:
    std::vector<Surface> mesh;
    float posX;
    float posY;
    float posZ;
    float yaw;
    float pitch;
    float roll;
};
