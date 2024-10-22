#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

class Surface
{
public:
    Surface(std::array<float,9> inVerticies, std::array<float,6> textureCoords, std::string inTextureMap);
    std::array<float,9> vertices;
    std::array<float,6> textureCoords;
    std::array<float,4> color;
    std::string textureMap;
private:
     
};

class Object 
{
public:
    Object(std::string objectFile, float inPosX, float inPosY, float inPosZ, float inYaw, float inPitch, float inRoll);
    void objectTranslate(float dX, float dY, float dZ);
    void objectRotate(float dYaw, float dPitch, float dRoll);
    std::vector<Surface> getMesh(float cameraX,float cameraY,float cameraZ,float cameraPitch,float cameraYaw,float cameraRoll);
    float getDistance(float cameraX,float cameraY,float cameraZ);
    float posX;
    float posY;
    float posZ;

private:
    std::vector<Surface> mesh;
    std::vector<Surface> createMeshFromFile(std::string filename);

    float x1;
    float y1;
    float z1;
    float x2;
    float y2;
    float z2;
    float x3;
    float y3;
    float z3;
    
    

    float nposX;
    float nposY;
    float nposZ;

    float yaw;
    float pitch;
    float roll;

    float sinYaw;
    float cosYaw;
    float sinPitch;
    float cosPitch;
    float sinRoll;
    float cosRoll;
};
