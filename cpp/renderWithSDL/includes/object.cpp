#include "object.hpp"

Object::Object(std::vector<Surface> inMesh, float inPosX, float inPosY, float inPosZ, float inYaw, float inPitch, float inRoll)
{
    mesh = inMesh;
    // XYZ is relative to camera
    posX = inPosX;
    posY = inPosY;
    posZ = inPosZ;

    // Rotations is relative to object itself
    yaw = inYaw;
    pitch = inPitch;
    roll = inRoll;
}

void Object::worldTranslate(float dX, float dY, float dZ)
{

    // Multiple translations should not be made at the same time
    // If they are the position translation will be made first
    posX += dX;
    posY += dY;
    posZ += dZ;
   
}

void Object::worldRotate(float dYaw, float dPitch, float dRoll)
{

    // Note: these rotations are relative to the camera & change the position of the object relative to the camera

    if(dYaw!=0) {
        posX = (posX*cos(dYaw))-(posZ*sin(dYaw));
        posZ = (posX*sin(dYaw))+(posZ*cos(dYaw));
    }
    if(dPitch!=0) {
        posZ = (posZ*cos(dPitch))-(posY*sin(dPitch));
        posY = (posZ*sin(dPitch))+(posY*cos(dPitch));
    }
    if(dPitch!=0) {
        posX = (posX*cos(dRoll))-(posY*sin(dRoll));
        posY = (posX*sin(dRoll))+(posY*cos(dRoll));
    }
}

std::vector<Surface> Object::getMesh()
{
    // Returns the mesh of the object (lot of parts to this)
    // For each vertex apply object rotation ( do this later )
    // For each vertex apply object relative position translation
    std::vector<Surface> returnMesh = mesh;
    for(auto it = returnMesh.begin(); it != returnMesh.end(); ++it) {
        // Add in portion to compensite for object rotation later (axis of rotation should be 0,0,0)
        it->vertices[0] += posX;
        it->vertices[1] += posX;
        it->vertices[2] += posX;
        it->vertices[3] += posY;
        it->vertices[4] += posY;
        it->vertices[5] += posY;
        it->vertices[6] += posZ;
        it->vertices[7] += posZ;
        it->vertices[8] += posZ;
    }

    return returnMesh;

    
}

Surface::Surface(std::array<float,9> inVerticies)
{
    // color in form r, g, b, alpha
    // verties in form x1,x2,x3,y1,y2,y3,z1,z2,z3
    vertices = inVerticies;
    //color = inColor;
}

